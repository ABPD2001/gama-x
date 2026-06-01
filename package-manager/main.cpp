#include "../core/types.hpp"
#include "./commands/basic.hpp"
#include "./commands/treebased.hpp"
#include "./commands/common.hpp"

#define HELP "help..."
#define VERSION "V1.0.0"

_GXPM_config_t configuration;
bool verbose = false;

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    const string treebased_verbs[5] = {"add", "list", "remove", "copy"};

    for (uint32_t i = 1; i < argc; i++)
    {
        const string flag = string(argv[i]);

        if (flag == "-v" || flag == "--verbose")
        {
            verbose = true;
            continue;
        }
        if (flag == "setup")
        {
            if (argc - i - 1)
            {
                print_error("'setup' verb does not accept any arguments!");
                exit(1);
            }
            setup();
            exit(0);
        }
        else if (flag == "start")
        {
            if (argc - i - 1 != 2)
            {
                print_error(string(argc - i - 1 > 2 ? "to much" : "to few") + "arguments for 'init'!");
                exit(1);
            }
            if (!fs::exists(argv[i + 1]) || !fs::is_directory(argv[i + 1]))
            {
                print_error("invalid repository path '" + string(argv[i + 1]) + "'!");
                exit(1);
            }
            i += 2;
            initialize(argv[i + 1], argv[i + 2]);
            exit(0);
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
            exit(0);
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
            i += 1;
            configure(argv[i + 1]);
            exit(0);
        }
        else if (flag == "merge")
        {
            vector<string> repos;
            _GXPM_repository_t output;

            for (uint32_t j = 0; i < argc - i - 1; j++)
            {
                const string flag = argv[i + j];
                if (flag == "-n" || flag == "--name")
                {
                    if (j == argc - i - 1)
                    {
                        print_error("'-n, --name' argument value missed!");
                        exit(1);
                    }
                    output.name = string(argv[i + j + 1]);
                }
                else if (flag == "-p" || flag == "--path")
                {
                    if (j == argc - i - 1)
                    {
                        print_error("'-p, --path' argument value missed!");
                        exit(1);
                    }
                    output.path = argv[i + j + 1];
                }
                else
                    repos.push_back(flag);
            }
            if (repos.size() < 1)
            {
                print_error("at least, two repositories are required!");
                exit(1);
            }
            if (!output.name.length())
            {
                print_error("'-n, --name' flag value missed!");
                exit(1);
            }
            if (!output.path.length())
            {
                print_error("'-p, --path' flag value missed!");
                exit(1);
            }

            merge(repos, output);
            exit(0);
        }
        else if (flag == "mergable")
        {
            vector<string> repos;
            bool stat = false;

            for (uint32_t j = 0; j < argc - i - 1; j++)
            {
                repos.push_back(string(argv[i + 1 + j]));
            }
            if (repos.size() < 2)
            {
                print_error("at least, two repositories are required!");
                exit(1);
            }

            mergable(repos, false, stat);
            exit(0);
        }
        else if (flag == "lint")
        {
            if (argc - i - 1 != 1)
            {
                print_error(((argc - i - 1 > 1) ? "to much" : "to few") + string(" arguments for 'lint' verb!"));
                exit(1);
            }

            lint(argv[i + 1]);
            exit(0);
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
        else if (flag == "add" || flag == "remove" || flag == "edit")
        {
            string argument, name, description, deps, repository, version, mainpoint_relative, target_repo;
            if (argc - i - 1 < 1)
            {
                print_error("target type of 'add' verb missed!");
                exit(1);
            }
            bool is_repo_based = string(argv[i + 1]) == "repo";
            if (string(argv[i + 1]) != "repo" && string(argv[i + 1]) != "lib")
            {
                print_error("invalid target type '" + string(argv[i + 1]) + "' for 'add' verb!");
                exit(1);
            }

            for (uint32_t j = 1; j < argc - i - 1; j++)
            {
                const string param = string(argv[i + 1 + j]);
                if (param == "-n" || param == "--name")
                {
                    if (!argc - i - j - 1)
                    {
                        print_error("'-n, --name' argument flag missed!");
                        exit(1);
                    }
                    name = argv[i + j + 1];
                }
                else if (param == "-d" || param == "--description")
                {
                    if (!argc - i - j - 1)
                    {
                        print_error("'-d, --description' argument flag missed!");
                        exit(1);
                    }
                    description = argv[i + j + 1];
                }
                else if (param == "p" || param == "--dependecies")
                {
                    if (!argc - i - j - 1)
                    {
                        print_error("'-p, --dependecies' argument flag missed!");
                        exit(1);
                    }
                    deps = argv[i + j + 1];
                }
                else if (param == "-V" || param == "--lib-version")
                {
                    if (!argc - i - j - 1)
                    {
                        print_error("'-V, --lib-version' argument flag missed!");
                        exit(1);
                    }
                    version = argv[i + j + 1];
                }
                else if (argument.empty())
                {
                    argument = argv[i + j + 1];
                }
                else
                {
                    print_error("invalid argument '" + string(argv[i + j + 1]) + "' for 'add' verb!");
                    exit(1);
                }
            }

            if (flag == "add")
            {
                if (is_repo_based)
                {
                    if (!(argument.length() && name.length() && description.length()))
                    {
                        print_error("name, path and description of repository are required!");
                        exit(1);
                    }
                    add_repo(argument, name, description);
                }
                else
                {
                    if (!(argument.length() && deps.length() && repository.length() && version.length() && mainpoint_relative.length() && name.length() && argument.length()))
                    {
                        print_error("name, target repository, mainpoint relative path, desciption, path and version of library are required!");
                        exit(1);
                    }
                    add_lib(split(deps, ','), repository, version, mainpoint_relative, name, description, argument, repository);
                }
                exit(0);
            }
            else if (flag == "remove")
            {
                if (is_repo_based)
                {
                    bool stat = false;
                    const bool exists = repository_exists(argument, argument, stat);
                    if (!stat)
                    {
                        print_error("failed to open metadata repositories data file!");
                        exit(1);
                    }
                    if (!exists)
                    {
                        print_error("invalid repository '" + argument + "' asked to remove!");
                        exit(1);
                    }
                    remove_repo(argument);
                }
                else
                {
                    bool stat = false;
                    const bool exists = repository_exists(repository, repository, stat);

                    if (!(argument.length() && repository.length()))
                    {
                        print_error("path/name of library and repository of library are required!");
                        exit(1);
                    }
                    if (!stat)
                    {
                        print_error("failed to open metadata repositories data file!");
                        exit(1);
                    }
                    if (!exists)
                    {
                        print_error("invalid repository '" + argument + "'!");
                        exit(1);
                    }
                    remove_lib(argument, repository);
                }
                exit(0);
            }
            else
            {
            }
        }
        else if (flag == "copy")
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