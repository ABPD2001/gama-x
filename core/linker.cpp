#include "./linker.hpp"

_GXL_::_GXL_(vector<_GXL_file_t> files, _GXL_config_t config)
{
    this->files = files;
    this->config = config;
    if (this->config.repository_path.length())
        this->repository = this->config.repository_path;
}
_GXL_::_GXL_() {};

void _GXL_::init(vector<_GXL_file_t> files, _GXL_config_t config)
{
    this->files = files;
    this->config = config;
    if (this->config.repository_path.length())
        this->repository = this->config.repository_path;
}

void _GXL_::_merge_files_()
{
    vector<_GXL_file_t> included_files;
    _GXLT_error_t error;

    for (_GXL_file_t f : this->files)
    {
        const vector<string> lines = split(f.content, '\n');
        const short int mainidx = f.content.find(".main"), modidx = f.content.find(".module");

        if ((mainidx == string::npos && modidx == string::npos) || (mainidx != string::npos && modidx != string::npos))
        {
            error = {f.name, "Invalid pre-processor type, its required to add main point or modular mode, only one mode!", "INVALID_FILE_TYPE", "syntax", 0};
            this->errors.push_back(error);
            continue;
        }
        if (f.content.find(".end") == string::npos)
        {
            error = {f.name, "Invalid pre-processor segmention!", "END_WORD_MISSED", "syntax", 0};
            this->errors.push_back(error);
            continue;
        }

        for (uint64_t i = 0; i < lines.size(); i++)
        {
            const string line = trim(lines[i]);
            const vector<string> parts = split(line, ' ');

            if (parts[0] == ".import")
                this->requested_libraries.push_back(parts[1]);
            else if (parts[0] == ".include")
            {
                fstream f;
                string content;
                fs::path p = parts[1];
                char ch;

                f.open(parts[1], ios::in);
                if (!f.is_open())
                {
                    error = {parts[1], "failed to open included path '" + parts[1] + "'!", "INCLUDE_FILE_FAILURE", "input-output", 0};
                    this->errors.push_back(error);
                }
                while (f.read(&ch, 1))
                {
                    content += ch;
                }
                f.close();

                _GXL_file_t file = {content, p.filename().string()};
                included_files.push_back(file);
            }

            else if (line[0] == '.')
            {
                this->merge_preprocessor += line;
                this->merge_preprocessor += "\n";
            }
            else
            {
                this->merge_instruction += line;
                this->merge_preprocessor += "\n";
            }

            this->merge_instruction = this->merge_instruction.substr(0, this->merge_instruction.length() - 1);
        }
    }

    if (included_files.size())
    {
        vector<_GXL_file_t>
            cpy = this->files;
        this->files = included_files;
        this->_merge_files_();
        this->files = cpy;
        this->files.insert(this->files.begin(), included_files.begin(), included_files.end());
    }
}

void _GXL_::_add_libraries_()
{
    vector<_GXPM_metadata_chunk_t> library_metadatas;
    vector<_GXPM_dependecies_t> library_dependecies;
    fstream libraries_config_f;
    _GXPM_metadata_chunk_t temp_struct;
    _GXPM_chunk_header_t temp_header;
    _GXLT_error_t error;

    const string metadata_path = (this->config.metadata_file_name.length() ? this->config.metadata_file_name : "/metadata.riff");

    libraries_config_f.open(metadata_path, ios::binary | ios::ate);
    if (!libraries_config_f.is_open())
    {
        error = {metadata_path, "Failed to open metadata file of repository!", "REPOSITORY_METADATA_FAILURE", "syntax", 0};
        this->errors.push_back(error);
        libraries_config_f.close();
        return;
    }

    const uint32_t file_size = libraries_config_f.tellg();

    libraries_config_f.seekg(0);
    while (file_size - libraries_config_f.tellg() > CHUNK_HEADER_SIZE)
    {
        libraries_config_f.read((char *)&temp_header, CHUNK_HEADER_SIZE);
        if (temp_header.id == "META")
        {
            libraries_config_f.read((char *)&temp_struct, META_CHUNK_SIZE);
            library_metadatas.push_back(temp_struct);
        }
        else if (temp_header.id == "DEPS")
        {
            string deps;
            char dep_id[8];
            char ch;

            libraries_config_f.read(dep_id, 8); // read 8 bytes as dependecy id.
            while (ch || deps.empty())
            {
                libraries_config_f.read(&ch, 1);
                deps += ch;
            }
            deps = deps.substr(0, deps.length() - 1); // remove last NULL terminator.

            const vector<string> deps_name = split(deps, ',');
            _GXPM_dependecies_t dep = {deps_name, dep_id};
            library_dependecies.push_back(dep);
        }
        else
        {
            error = {metadata_path, "Failed to read chunk of repository metadata file!", "CHUNK_PARSE_FAILURE", "input-output", libraries_config_f.tellg()};
            this->errors.push_back(error);
            break;
        }
    }
    libraries_config_f.close();
    if (this->errors.size())
        return;

    if (this->config.dependecy_check)
    {
        for (uint32_t i = 0; i < library_metadatas.size(); i++)
        {
            for (uint32_t j = 0; j < library_dependecies.size(); j++)
            {
                if (library_dependecies[j].id == library_metadatas[i].dep_id)
                {
                    this->requested_libraries.insert(this->requested_libraries.begin(), library_dependecies[j].dependecies.begin(), library_dependecies[j].dependecies.end());
                    break;
                }
            }
        }
    }

    for (string req_lib : this->requested_libraries)
    {
        bool found = false;

        for (_GXPM_metadata_chunk_t md : library_metadatas)
        {
            if (md.libname == req_lib)
            {
                fstream lib_f;
                _GXL_file_t file;
                const string fpath = this->repository + '/' + md.pathname + '/' + md.mainpoint_relative;
                char ch;

                found = true;

                lib_f.open(fpath, ios::in);
                if (!lib_f.is_open())
                {
                    error = {fpath, "Failed to read!", "LIBRARY_MAIN_POINT_FAILURE", "input-output", 0};
                    this->errors.push_back(error);
                    break;
                }

                file.name = '<' + md.libname + '>';
                while (lib_f.read(&ch, 1))
                {
                    file.content += ch;
                }
                lib_f.close();
                this->files.push_back(file);
            }
        }
        if (!found)
        {
            error = {"", "Invalid library '" + req_lib + "' requested!", "LIBRARY_NOT_FOUND", "importation", 0};
            this->errors.push_back(error);
        }
    }
}

bool _GXL_::merge()
{
    if (this->config.repository_check)
    {
        this->_add_libraries_();
        if (this->errors.size())
            return false;
    }
    this->_merge_files_();

    if (this->errors.size())
        return false;
    this->merged = this->merge_preprocessor + this->merge_instruction;

    return true;
}