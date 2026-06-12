#include "../core/types.hpp"
#include "./commands/basic.hpp"
#include "./commands/treebased.hpp"
#include "./commands/common.hpp"
#include "./help.hpp"

#define VERSION "V1.0.0"

string GXPM_LOCAL_PATH;
string REPOSITORIES_PATH;
string _HOME_;
bool verbose = false;

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    bool is_root = false, username_set = false;

#ifdef __linux__
    _HOME_ = "/home";
    GXPM_LOCAL_PATH = string(getenv("HOME")) + "/.gxpm";
    REPOSITORIES_PATH = string(getenv("HOME")) + "/.gxpm/repos.riff";
    is_root = getenv("HOME") == "/";
#endif
#ifdef _WIN32
    _HOME_ = "C:\\Users";
    GXPM_LOCAL_PATH = string(getenv("USERPROFILE")) + "/.gxpm";
    REPOSITORIES_PATH = string(getenv("USERPROFILE")) + "/.gxpm/repos.riff";
    is_root = getenv("C:\\") == "/";
#endif

    const string treebased_verbs[6] = {"add", "list", "remove", "copy", "edit"};

    for (uint32_t i = 1; i < argc; i++)
    {
        const string flag = string(argv[i]);

        if (flag == "-v" || flag == "--verbose")
        {
            verbose = true;
            continue;
        }
        if (flag == "-u" || flag == "--username")
        {
            if (i == argc - 1)
            {
                print_error("'-u, --username' argument value missed!");
                exit(1);
            }
            username_set = true;
            GXPM_LOCAL_PATH = _HOME_ + string(argv[i + 1]) + "/.gxpm";
            REPOSITORIES_PATH = GXPM_LOCAL_PATH + "/repos.riff";
            i++;
            continue;
        }
        else if (is_root && !username_set)
        {
            print_error("'-u, --username' flag is required when admin permision used.");
            exit(1);
        }
        if (flag == "setup")
        {
            bool local = false;
            bool local_ignore = false;

            if (argc - i - 1 > 2)
            {
                print_error("to much arguments for 'setup'!");
                exit(1);
            }
            for (uint32_t j = i + 1; j < argc; j++)
            {
                const string param = string(argv[j]);
                if (param == "-l" || param == "--local")
                    local = true;
                else if (param == "-nl" || param == "--skip-locals")
                    local_ignore = true;

                else
                {
                    print_error("invalid argument '" + param + "' for 'setup' verb!");
                    exit(1);
                }
            }

            setup(local, local_ignore);
            exit(0);
        }
        else if (flag == "start")
        {
            if (argc - i - 1 != 1)
            {
                print_error(string(argc - i - 1 > 1 ? "to much" : "to few") + " arguments for 'start'!");
                exit(1);
            }
            if (!fs::exists(argv[i + 1]) || !fs::is_directory(argv[i + 1]))
            {
                print_error("invalid repository path '" + string(argv[i + 1]) + "'!");
                exit(1);
            }
            initialize(argv[i + 1]);
            i += 2;
            exit(0);
        }
        else if (flag == "reset")
        {
            const string param = argv[i + 1];
            bool rage = false;

            if (argc - i - 1 > 1)
            {
                print_error("to much arguments for 'reset' verb!");
                exit(1);
            }

            for (uint32_t j = i + 1; j < argc; j++)
            {

                if (param == "-r" || param == "--rage")
                    rage = true;

                else
                {
                    print_error("invalid flag '" + param + "' for 'reset' verb!");
                    exit(1);
                }
            }

            reset(rage);
            exit(0);
        }
        else if (flag == "create-mainpoint")
        {
            string path;
            string output, repository = "global";
            bool ignore_dependecies, ignore_repository;

            if (i == argc - 1)
            {
                print_error("path to create main point missed!");
                exit(1);
            }
            path = argv[i + 1];
            if (argc - i - 1 > 6)
            {
                print_error("to much argument 'create-mainpoint'!");
                exit(1);
            }
            for (uint32_t j = i + 2; j < argc; j++)
            {
                const string param = argv[j];
                if (param == "-r" || param == "--repository")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-r, --repository' argument value missed!");
                        exit(1);
                    }
                    repository = argv[j + 1];
                }
                else if (param == "-o" || param == "--output")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-o, --output' argument value missed!");
                        exit(1);
                    }
                    output = argv[j + 1];
                }
                else if (param == "-nd" || param == "--ignore-dependecies")
                    ignore_dependecies = true;
                else if (param == "-nr" || param == "--ignore-repository")
                    ignore_repository = true;
            }
            if (verbose && repository == "global")
                cout << "[using global repository]\n";
            create_mainpoint(path, output, repository, ignore_dependecies, ignore_repository);
        }
        else if (flag == "list")
        {
            string target_type;
            string repository;
            bool local_only = false;

            if (i == argc - 1)
            {
                print_error("target type missed for 'list' verb!");
                exit(1);
            }
            if (argc - i - 1 > 5)
            {
                print_error("to much arguments for 'list' verb!");
                exit(1);
            }

            target_type = argv[i + 1];
            if (target_type != "lib" && target_type != "repo")
            {
                print_error("invalid target type '" + target_type + "' instead of 'repo'/'lib' for 'list' verb!");
                exit(1);
            }
            for (uint32_t j = i + 2; j < argc; j++)
            {
                const string param = argv[j];
                if ((param == "-r" || param == "--repository") && target_type == "lib")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-r, --repository' argument value missed!");
                        exit(1);
                    }
                    repository = argv[j + 1];
                }
                else if ((param == "-l" || param == "--locally") && target_type == "repo")
                    local_only = true;
                else
                {
                    print_error("invalid argument '" + param + "' for 'list " + target_type + "' verb!");
                    exit(1);
                }
            }
            if (target_type == "lib" && !repository.length())
            {
                print_error("repository name for 'list lib' is required!");
                exit(1);
            }

            if (target_type == "lib")
                list_libs(repository);
            else
                list_repos(local_only);
        }
        else if (flag == "validate")
        {
            string repository;
            if (i == argc - 1)
            {
                print_error("repository to validate missed!");
                exit(1);
            }
            if (argc - i - 1 > 1)
            {
                print_error("to much arguments for 'validate' verb!");
                exit(1);
            }
            repository = argv[i + 1];
            validate_repository(repository);
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
            cout << help << '\n';
            exit(0);
        }
        else if (flag == "version")
        {
            cout << VERSION << '\n';
            exit(0);
        }
        else if (flag == "copy" || flag == "add" || flag == "remove" || flag == "edit")
        {
            string argument, name, description, deps, repository, version, path, mainpoint_relative, target;
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

            for (uint32_t j = i + 2; j < argc; j++)
            {
                const string param = string(argv[j]);
                if (param == "-t" || param == "--target")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-t, --target' argument flag missed!");
                        exit(1);
                    }
                    target = argv[j + 1];
                    j++;
                }
                else if (param == "-p" || param == "--path")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-p, --path' argument flag missed!");
                        exit(1);
                    }
                    path = argv[j + 1];
                    j++;
                }
                else if (param == "-r" || param == "--repository")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-r, --repository' argument flag missed!");
                        exit(1);
                    }
                    repository = argv[j + 1];
                    j++;
                }
                else if (param == "-m" || param == "--mainpoint-relative")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-m, --mainpoint-relative' argument flag missed!");
                        exit(1);
                    }
                    mainpoint_relative = argv[j + 1];
                    j++;
                }
                else if (param == "-n" || param == "--name")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-n, --name' argument flag missed!");
                        exit(1);
                    }
                    name = argv[j + 1];
                    j++;
                }
                else if (param == "-d" || param == "--description")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-d, --description' argument flag missed!");
                        exit(1);
                    }
                    description = argv[j + 1];
                    j++;
                }
                else if (param == "-dp" || param == "--dependecies")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-p, --dependecies' argument flag missed!");
                        exit(1);
                    }
                    deps = argv[j + 1];
                    j++;
                }
                else if (param == "-V" || param == "--lib-version")
                {
                    if (j == argc - 1)
                    {
                        print_error("'-V, --lib-version' argument flag missed!");
                        exit(1);
                    }
                    version = argv[j + 1];
                    j++;
                }
                else if (argument.empty())
                {
                    argument = argv[j];
                    if (flag == "copy")
                        argument += ',';
                }
                else
                {
                    print_error("invalid argument '" + string(argv[j]) + "' for 'add' verb!");
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
                    add_repo(name, argument, description);
                }
                else
                {
                    if (!(argument.length() && repository.length() && version.length() && mainpoint_relative.length() && name.length() && argument.length()))
                    {
                        print_error("name, target repository, mainpoint relative path, description, path and version of library are required!");
                        exit(1);
                    }
                    add_lib(deps, repository, version, mainpoint_relative, name, description, argument, repository);
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
                else if (flag == "remove")
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
            else if (flag == "copy")
            {
                if (is_repo_based)
                {
                    const vector<string> repos = split(argument.substr(0, argument.length() - 1), ',');
                    bool valid = true;
                    bool stat = false;

                    if (!argument.length() || !target.length())
                    {
                        print_error("repositories to copy and target path are required!");
                        exit(1);
                    }
                    copy_repos(repos, target);
                }
                else
                {
                    if (!argument.length() || !target.length() || !repository.length())
                    {
                        print_error("libraries to copy and target repository and libraries repository are requied!");
                        exit(1);
                    }
                    const vector<string> libs = split(argument.substr(0, argument.length() - 1), ',');
                    bool stat = false;
                    bool valid = true;
                    const bool exists = repository_exists(target, target, stat);
                    if (!stat)
                    {
                        print_error("repository '" + target + "' not found!");
                        exit(1);
                    }
                    for (string l : libs)
                    {
                        const bool libexists = library_exists(l, l, repository, stat);
                        if (!stat)
                        {
                            print_error("failed to open '" + repository + "' metadata file!");
                            exit(1);
                        }
                        if (!libexists)
                        {
                            valid = false;
                            print_error("library '" + l + "' from does not exists at '" + repository + "' repository!");
                        }
                    }
                    if (!valid)
                        exit(1);
                    copy_libs(libs, repository, target);
                }
            }
            else
            {
                if (is_repo_based)
                {
                    _GXPM_repository_t edits = {description, path, name};
                    edit_repo(edits, argument);
                    exit(0);
                }
                else
                {
                    if (repository.empty())
                    {
                        print_error("repository of library is required!");
                        exit(1);
                    }
                    _GXPM_metadata_t lib = {description, mainpoint_relative, path, name, version};
                    edit_lib(lib, argument, repository);
                    exit(0);
                }
            }
        }
        else
        {
            print_error("unkown verb '" + flag + "'!");
            exit(1);
        }
    }

    return 0;
}
