#include "../core/types.hpp"
#include "./commands/basic.hpp"
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
            continue;
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
            continue;
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
            continue;
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
        }
        else if (flag == "lint")
        {
            if (argc - i - 1 != 1)
            {
                print_error(((argc - i - 1 > 1) ? "to much" : "to few") + string(" arguments for 'lint' verb!"));
                exit(1);
            }

            lint(argv[i + 1]);
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
        else if (flag == "add")
        {
        }
        else if (flag == "edit")
        {
        }
        else if (flag == "remove")
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