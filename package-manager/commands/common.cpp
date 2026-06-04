#include "./common.hpp"

void print_error(string text)
{
    cout << "GXPM: " << text << '\n';
}

bool create_8byte_id(char bytes[8], uint32_t &lvl)
{
    switch (lvl)
    {
    case 0:
        for (uint32_t i = 0; i < 8; i++)
        {
            if (bytes[i] != 127)
            {
                bytes[i] += 1;
                return true;
            }
            else if (i == 7)
            {
                lvl++;
                for (uint32_t j = 0; j < 8; j++)
                {
                    bytes[j] = 0;
                }
                return create_8byte_id(bytes, lvl);
            }
        }
    case 1:
        bytes[0] = 127;
        for (uint32_t i = 1; i < 8; i++)
        {
            if (bytes[i] != 127)
            {
                bytes[i] += 1;
                return true;
            }
            else if (i == 7)
            {

                lvl++;
                for (uint32_t j = 0; j < 8; j++)
                {
                    bytes[j] = 127;
                }
                return create_8byte_id(bytes, lvl);
            }
        }
    case 2:
        for (uint32_t i = 0; i < 7; i++)
        {
            if (bytes[6 - i])
            {
                bytes[6 - i] -= 1;
                return true;
            }
            else if (!(i - 6))
            {
                return false;
            }
        }
    default:
        return false;
    }
};

bool repository_exists(string name, string path, bool &stat)
{
    for (_GXPM_repository_t r : read_repositories_chunk(REPOSITORIES_PATH, stat))
    {
        if (!stat)
            return false;
        if (name.length())
        {
            if (r.name == name)
                return true;
        }
        else if (path.length())
        {
            if (r.path == absolute_path(path))
                return true;
        }
    }
    return false;
}
_GXPM_repository_t find_repository(string name, string path, bool &found, bool &stat)
{
    const _GXPM_repository_t null_repo = {"", "", "", 0};
    for (_GXPM_repository_t r : read_repositories_chunk(REPOSITORIES_PATH, stat))
    {
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
        if (name.length())
        {
            if (r.name == name)
            {
                found = true;
                return r;
            }
        }
        else if (path.length())
        {
            if (r.path == path)
            {
                found = true;
                return r;
            }
        }
    }
    found = false;
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    return null_repo;
}

bool library_exists(string name, string path, string repository, bool &stat)
{
    bool found = false;
    fs::path p;
    p = find_repository(repository, repository, found, stat).path;
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    if (!found)
    {
        print_error("repository '" + repository + "' not found!");
        exit(1);
    }
    const string mpath = fs::absolute(p).string() + "/metadata.riff";

    for (_GXPM_metadata_t m : read_metadata_chunks(mpath, stat))
    {
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
        if (m.libname == name || (fs::exists(path) && m.pathname == fs::absolute(path)))
        {
            return true;
        }
    }
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    return false;
}

vector<_GXL_file_t> read_files(string path)
{
    vector<_GXL_file_t> files;

    for (auto &item : fs::directory_iterator(path))
    {
        if (item.is_regular_file() && item.path().string().find(".s") != string::npos)
        {
            fstream f;
            _GXL_file_t file;
            char ch;

            file.name = absolute_path(item.path().string());
            if (verbose)
                cout << "[processing '" << file.name << "' ...]\n";

            f.open(file.name, ios::in);
            if (!f.is_open())
            {
                print_error("failed to open file '" + file.name + "'!");
                exit(1);
            }
            while (f.read(&ch, 1))
            {
                file.content += ch;
            }
            f.close();
            files.push_back(file);
        }
        if (item.is_directory())
        {
            vector<_GXL_file_t> tmp = read_files(path);
            files.insert(files.end(), tmp.begin(), tmp.end());
        }
    }
    return files;
}

void print_linker_errors(vector<_GXLT_error_t> errors)
{
    string cpy = "Error in <FILE> at line <LINE>: <MESSAGE>\ntype <TYPE>, cause <CAUSE>";
    ;
    string temp = cpy;
    const string forms[] = {"<FILE>", "<MESSAGE>", "<CAUSE>", "<TYPE>", "<LINE>"};

    for (_GXLT_error_t err : errors)
    {
        const string vals[] = {err.file, err.message, err.cause, err.type, to_string(err.line + 2)};
        for (uint32_t i = 0; i < 5; i++)
        {
            if (temp.find(forms[i]) == string::npos)
                continue;
            temp.replace(temp.find(forms[i]), forms[i].length(), vals[i]);
        }

        cout << temp << "\n";
        temp = cpy;
    }
}