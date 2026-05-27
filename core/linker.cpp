#include "./linker.hpp"

_GXL_::_GXL_(vector<_GXL_file_t> files, _GXL_config_t config, string repository)
{
    this->files = files;
    this->repository = repository;
    this->config = config;
}
_GXL_::_GXL_() {};

void _GXL_::_merge_files_()
{
    _GXL_error_t error;

    for (_GXL_file_t f : this->files)
    {
        const vector<string> lines = split(f.content, '\n');
        const short int mainidx = f.content.find(".main"), modidx = f.content.find(".module");

        if ((mainidx == string::npos && modidx == string::npos) || (mainidx != string::npos && modidx != string::npos))
        {
            error = {f.name, "Invalid pre-processor type, its required to add main point or modular mode, only one mode!", "type", 0};
            this->errors.push_back(error);
            continue;
        }
        if (f.content.find(".end") == string::npos)
        {
            error = {f.name, "Invalid pre-processor segmention!", "syntax", "END_WORD_MISSED", 0};
            this->errors.push_back(error);
            continue;
        }

        for (uint64_t i = 0; i < lines.size(); i++)
        {
            const string line = trim(lines[i]);
            const vector<string> parts = split(line, ' ');

            if (parts[0] == ".import")
                this->requested_libraries.push_back(parts[1]);
            else if (line[0] == '.')
                this->merge_preprocessor += line + '\n';
            else
                this->merge_instruction += line + '\n';

            this->merge_instruction.resize(this->merge_instruction.length() + 1);
            this->merge_instruction[this->merge_instruction.length() - 1] = '\0';
        }
    }
}

void _GXL_::_add_libraries_()
{
    vector<_GXL_library_metadata_chunk_t> library_metadatas;
    vector<_GXL_library_dependecy_t> library_dependecies;
    fstream libraries_config_f;
    _GXL_library_metadata_chunk_t temp_struct;
    _GXL_library_metadata_chunk_header_t temp_header;
    _GXL_error_t error;

    libraries_config_f.open(this->repository + "/metadata.riff", ios::binary | ios::ate);
    if (!libraries_config_f.is_open())
    {
        error = {this->repository + "/metadata.riff", "Failed to open metadata file of repository!", "syntax", "REPOSITORY_METADATA_FAILURE", 0};
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
            while (ch != NULL || deps.empty())
            {
                libraries_config_f.read(&ch, 1);
                deps += ch;
            }
            deps = deps.substr(0, deps.length() - 1); // remove last NULL terminator.

            const vector<string> deps_name = split(deps, ',');
            _GXL_library_dependecy_t dep = {deps_name, dep_id};
            library_dependecies.push_back(dep);
        }
    }
    libraries_config_f.close();

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

    vector<string> libraries_to_insert;
    for (uint32_t i = 0; i < this->requested_libraries.size(); i++)
    {
        if (!includes<string>(libraries_to_insert, this->requested_libraries[i]))
        {
            libraries_to_insert.push_back(this->requested_libraries[i]);
        }
    }

    this->requested_libraries = libraries_to_insert;

    for (string req_lib : this->requested_libraries)
    {
        bool found = false;

        for (_GXL_library_metadata_chunk_t md : library_metadatas)
        {
            if (md.libname == req_lib)
            {
                fstream lib_f;
                _GXL_file_t file;
                const string fpath = this->repository + '/' + md.pathname + '/' + md.mainpoint_relative;
                string tmp_line;

                found = true;

                lib_f.open(fpath, ios::in);
                if (!lib_f.is_open())
                {
                    error = {fpath, "Failed to read!", "input-output", "LIBRARY_MAIN_POINT_FAILURE", 0};
                    this->errors.push_back(error);
                    break;
                }

                file.name = '<' + md.libname + '>';
                while (getline(lib_f, tmp_line))
                {
                    file.content += tmp_line + '\n';
                }
                file.content.resize(file.content.length() + 1);
                file.content[file.content.size() - 1] = '\0';
                lib_f.close();
            }
        }
        if (!found)
        {
            error = {"", "Invalid library '" + req_lib + "' requested!", "importation", "LIBRARY_NOT_FOUND", 0};
            this->errors.push_back(error);
        }
        else
            found = false;
    }
}

string _GXL_::merge()
{
    this->_add_libraries_();
    this->_merge_files_();
    return this->merge_preprocessor + this->merge_instruction;
}