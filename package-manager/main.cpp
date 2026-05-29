#include "../core/types.hpp"
#include "./structures.hpp"

#define HELP "help..."
#define VERSION "V1.0.0"

#define INIT_CONFIGURATION_PATH "/etc/gxpm"
#define CONFIG_PATH "/etc/gxpm/config.cnf"
#define REPOSITORIES_PATH "/etc/gxpm/repos.riff"
#define GLOBAL_REPOSITORIES_PATH "/etc/gxpm/globals"

#define DEFAULT_CONFIG "metadata_filename=metadata.riff"

_GXPM_config_t configuration;

void print_error(string text);
void _wrtie_config_(_GXPM_config_t config);
void reset(bool rage);
void configure(string filepath);
void initialize(string init_repo_name, string init_repo_desc, string init_repo_path);
int main(int argc, char *argv[]);

void print_error(string text)
{
    cout << "GXPM: " << text << '\n';
}

void _wrtie_config_(_GXPM_config_t config)
{
    fstream cnf_f;
    if (!fs::exists(CONFIG_PATH))
    {
        print_error("configuration file does not exists!");
        exit(1);
    }
    cnf_f.open(CONFIG_PATH, ios::out | ios::trunc);
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

void reset(bool rage)
{
    try
    {
        if (!fs::exists(INIT_CONFIGURATION_PATH))
            exit(0);

        if (rage)
        {
            fs::remove_all(INIT_CONFIGURATION_PATH);
            fs::create_directory(INIT_CONFIGURATION_PATH);
        }
        else
        {
            fstream f;
            f.open(CONFIG_PATH, ios::out | ios::trunc);
            if (!f.is_open())
            {
                print_error("failed to open config file!");
                exit(1);
            }
            f << DEFAULT_CONFIG;
            f.close();
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to initialize, error from '" + err.path1().string() + "', " + string(err.what()) + '!');
        exit(1);
    }
}
void configure(string filepath)
{
    fstream f;
    string content;
    char ch;

    if (!fs::exists(filepath))
    {
        print_error("config file '" + filepath + "' does not exists!");
        exit(1);
    }
    f.open(filepath, ios::in);
    if (!f.is_open())
    {
        print_error("failed to open config file '" + filepath + "'!");
        exit(1);
    }

    while (f.read(&ch, 1))
    {
        content += ch;
    }

    const vector<argument_t> arguments = parse_arguments(content);
    const string valid_arg_keys[1] = {"metadata_filename"};

    for (uint32_t i = 0; i < arguments.size(); i++)
    {
        if (!includes_arr<const string, 1>(valid_arg_keys, arguments[i].key))
        {
            print_error("invalid config file '" + filepath + "'!");
            exit(1);
        }
        else if (arguments[i].key == "metadata_filename")
        {
            configuration.metadata_filename = arguments[i].value;
        }
    }
    f.close();

    _wrtie_config_(configuration);
}

void initialize(string init_repo_name, string init_repo_desc, string init_repo_path)
{
    bool stat = false;

    try
    {
        if (!fs::exists(INIT_CONFIGURATION_PATH))
        {
            fs::create_directory(INIT_CONFIGURATION_PATH);
            fs::create_directory(GLOBAL_REPOSITORIES_PATH);
        }
        if (!fs::is_directory(INIT_CONFIGURATION_PATH))
        {
            fs::remove(INIT_CONFIGURATION_PATH);
            fs::create_directories(INIT_CONFIGURATION_PATH);
            fs::create_directories(GLOBAL_REPOSITORIES_PATH);
        }
        if (fs::exists(GLOBAL_REPOSITORIES_PATH))
        {
            if (!fs::is_directory(GLOBAL_REPOSITORIES_PATH))
            {
                fs::remove(GLOBAL_REPOSITORIES_PATH);
                fs::create_directory(GLOBAL_REPOSITORIES_PATH);
            }
            const bool glob_repos_validate = validate_chunks(GLOBAL_REPOSITORIES_PATH + configuration.metadata_filename, stat);
            if (!stat)
            {
                print_error("failed to validate metadata file in global repositories '" + string(GLOBAL_REPOSITORIES_PATH) + configuration.metadata_filename + "'!");
                exit(1);
            }
            if (fs::is_directory(GLOBAL_REPOSITORIES_PATH) && !glob_repos_validate)
            {
                print_error("invalid repository metadata file in global repositories path!");
                exit(1);
            }
        }

        if (!fs::exists(init_repo_path) || !fs::is_directory(init_repo_path))
        {
            print_error("initial repository path isnt valid!");
            exit(1);
        }

        if (!fs::exists(REPOSITORIES_PATH))
        {
            _GXPM_repository_t init_repos = {init_repo_desc, init_repo_path + string("/") + init_repo_name, init_repo_name, 0};
            write_repos_chunks({init_repos}, REPOSITORIES_PATH, stat);
            if (!stat)
            {
                print_error("failed to initalize repositories file!");
                exit(1);
            }
        }

        const bool repo_validate = validate_chunks(REPOSITORIES_PATH, stat);
        if (!stat)
        {
            print_error("failed to validate '" + string(REPOSITORIES_PATH) + "' repositories file!");
            exit(1);
        }
        if (fs::exists(REPOSITORIES_PATH) && !repo_validate)
        {
            fs::remove(REPOSITORIES_PATH);
            _GXPM_repository_t init_repos = {init_repo_desc, init_repo_path + string("/") + init_repo_name, init_repo_name, 0};
            write_repos_chunks({init_repos}, REPOSITORIES_PATH, stat);
            if (!stat)
            {
                print_error("failed to initalize repositories file!");
                exit(1);
            }
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to initialize, error from '" + err.path1().string() + "', " + string(err.what()) + '!');
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    const string treebased_verbs[4] = {"add", "lint", "list", "remove"};

    for (uint32_t i = 1; i < argc; i++)
    {
        const string flag = string(argv[i]);

        if (flag == "init")
        {
            string path = argv[i + 3];
            if (argc - i - 1 < 3)
            {
                print_error("arguments missed for 'init' verb!");
                exit(1);
            }
            if (path == "global")
                path = GLOBAL_REPOSITORIES_PATH;
            initialize(argv[i + 1], argv[i + 2], path);
            i += 4;
        }
        else if (flag == "reset")
        {
            bool rage = false;
            if (argc - i - 1 >= 1)
            {
                if (argc - i - 1 > 1)
                {
                    print_error("to much arguments for 'reset' verb!");
                    exit(1);
                }
                else if (string(argv[i + 1]) == "-r" || string(argv[i + 1]) == "--rage")
                {
                    rage = true;
                }
                else
                {
                    print_error("invalid flag '" + string(argv[i + 1]) + "' for 'reset' verb!");
                    exit(1);
                }
            }
            reset(rage);
        }
        else if (flag == "config")
        {
            if (argc - i - 1 != 1)
            {
                if (argc - i - 1 > 1)
                {
                    print_error("to much arguments for 'config' verb!");
                    exit(1);
                }
                else if (!argc - i - 1)
                {
                    print_error("arguments for 'config' verb missed!");
                    exit(1);
                }
            }
            configure(argv[i + 1]);
        }
        else if (flag == "merge")
        {
        }
        else if (flag == "help")
        {
            cout << HELP << '\n';
            exit(0);
        }
        else if (flag == "version")
        {
            cout << VERSION << '\n';
            exit(0);
        }
        else if (includes_arr<const string, 4>(treebased_verbs, flag))
        {
        }
        else
        {
            print_error("unkown verb '" + flag + "'!");
            exit(1);
        }
    }

    return 0;
}