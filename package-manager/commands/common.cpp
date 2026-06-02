#include "./common.hpp"

void print_error(string text)
{
    cout << "GXPM: " << text << '\n';
}
bool _validate_config_(string content)
{
    vector<argument_t> args = parse_arguments(content);
    const string valid_args[1] = {"metadata_filename"};

    for (argument_t a : args)
    {
        if (!includes_arr<const string, 1>(valid_args, a.key))
        {
            return false;
        }
        if (a.key == "metadata_filename" && !valid_name(a.value))
        {
            return false;
        }
    }
    return true;
}
void _wrtie_config_(_GXPM_config_t config)
{
    fstream cnf_f;
    if (!fs::exists(CONFIGURATION_PATH))
    {
        print_error("configuration file does not exists!");
        exit(1);
    }
    cnf_f.open(CONFIGURATION_PATH, ios::out | ios::trunc);
    if (!cnf_f.is_open())
    {
        print_error("failed to open configuration file!");
        exit(1);
    }

    string format = "metadata_filename=<METADATAFILENAME>";
    format.replace(format.find("<METADATAFILENAME>"), string("<METADATAFILENAME>").length(), config.metadata_filename);
    cnf_f << format;
    cnf_f.close();
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
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    if (!stat)
        return false;
    for (_GXPM_repository_t r : repos)
    {
        if (name.length())
        {
            if (r.name == name)
                return true;
        }
        else if (path.length())
        {
            if (r.path == path)
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
        else if (path.length() && fs::exists(path))
        {
            if (r.path == absolute_path(path))
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
    const string mpath = absolute_path(p.string()) + "/" + (configuration.metadata_filename.length() ? configuration.metadata_filename : "metadata.riff");

    for (_GXPM_metadata_t m : read_metadata_chunks(mpath, stat))
    {
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
        if (m.libname == name || (fs::exists(path) && m.pathname == absolute_path(path)))
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