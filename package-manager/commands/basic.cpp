#include "./basic.hpp"

void reset(bool rage)
{
    try
    {
        if (!fs::exists(GXPM_PATH) && !fs::exists(GXPM_LOCAL_PATH))
            exit(0);

        if (rage)
        {
            fs::remove_all(GXPM_PATH);
            if (verbose)
                cout << "['" << GXPM_PATH << "' deleted]\n";

            fs::remove_all(GXPM_LOCAL_PATH);
            if (verbose)
                cout << "['" << GXPM_LOCAL_PATH << "' deleted]\n";
        }
        else
        {
            bool stat = false;
            write_initial_chunk(REPOSITORIES_PATH, true, stat);
            if (!stat)
            {
                print_error("failed to reset repositories (local) data file!");
                exit(1);
            }
            if (verbose)
                cout << "['" << REPOSITORIES_PATH << "' file reseted]\n";
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to initialize, error from '" + err.path1().string() + "', " + string(err.what()) + '!');
        exit(1);
    }
}
void initialize(string path)
{
    fstream f;
    bool stat;

    if (fs::exists(path) && !fs::is_directory(path))
    {
        try
        {
            fs::remove(path);
        }
        catch (const fs::filesystem_error &err)
        {
            print_error("failed to initialize, error from '" + err.path1().string() + "', " + string(err.what()) + '!');
            exit(1);
        }
    }
    f.open(path + "/metadata.riff", ios::out);
    if (!f.is_open())
    {
        print_error("failed to append file '" + path + "/metadata.riff" + "' !");
        exit(1);
    }
    _GXPM_chunk_header_t header = {{'I', 'N', 'I', 'T'}, 0};
    f.write((char *)&header, sizeof(_GXPM_chunk_header_t));
    f.close();
}

void setup(bool local, bool local_ignore)
{
    fstream f;
    _GXPM_repository_t glob_def_metadata_simple = {"global repository, libraries shared betwen users are accessable here.", GLOBAL_REPOSITORY_PATH, "global", 0};
    const _GXPM_repository_chunk_t glob_default_metadata = to_repository_chunk(glob_def_metadata_simple);
    const vector<_GXPM_repository_chunk_t> default_repos = {glob_default_metadata};
    bool stat = false;

    try
    {
        string content;
        char ch;

        // GLOBAL SETUP
        if (!local)
        {
            const string glob_repo_metadata = GLOBAL_REPOSITORY_PATH + string("/metadata.riff");

            if (!fs::exists(GXPM_PATH))
            {
                fs::create_directory(GXPM_PATH);
                fs::create_directory(GLOBAL_REPOSITORY_PATH);
            }
            else if (!fs::is_directory(GXPM_PATH))
            {
                print_error("invalid gxpm configuration directory!\nnote: its better to use 'reset -r' verb, then initialize it.");
                exit(1);
            }

            if (!fs::exists(GLOBAL_REPOSITORY_PATH))
            {
                fs::create_directory(GLOBAL_REPOSITORY_PATH);
                if (verbose)
                    cout << "['" << GLOBAL_REPOSITORY_PATH << "' created]\n";
            }
            else
            {
                if (fs::exists(glob_repo_metadata))
                {
                    const bool globals_metadata_valid = validate_chunks(glob_repo_metadata, stat);

                    if (!stat)
                    {
                        print_error("failed to validate global repository metadata file '" + glob_repo_metadata + "'!");
                        exit(1);
                    }
                    if (!globals_metadata_valid)
                    {
                        print_error("global repository metadata file does not valid!\nnote: it can be fixed with 'reset -r'.");
                        exit(1);
                    }
                }
                else
                {
                    write_initial_chunk(glob_repo_metadata, true, stat);
                    if (!stat)
                    {
                        print_error("failed to initalize global repository metadata file!");
                        exit(1);
                    }
                }
            }

            if (verbose)
                cout << "['" << glob_repo_metadata << "' chunks are valid]\n";
        }

        if (local_ignore)
            exit(0);

        // LOCAL SETUP
        if (!fs::exists(GXPM_LOCAL_PATH))
        {
            fs::create_directory(GXPM_LOCAL_PATH);
            if (verbose)
                cout << "['" << GXPM_LOCAL_PATH << "' created]\n";
        }
        else if (!fs::is_directory(GXPM_LOCAL_PATH))
        {

            fs::remove(GXPM_LOCAL_PATH);
            fs::create_directory(GXPM_LOCAL_PATH);
            if (verbose)
                cout << "['" << GXPM_LOCAL_PATH << "' deleted and recreated, because wasnt a valid directory]\n";
        }

        if (!fs::exists(REPOSITORIES_PATH))
        {
            write_initial_chunk(REPOSITORIES_PATH, true, stat);
            if (!stat)
            {
                print_error("failed to append repositories data (local) file!");
                exit(1);
            }
        }
        else
        {
            const bool is_valid = validate_chunks(REPOSITORIES_PATH, stat);
            if (!stat)
            {
                print_error("failed to validate repositories data (local) file!");
                exit(1);
            }
            if (!is_valid)
            {
                print_error("invalid repositories data file!");
                exit(1);
            }
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to initialize, error from '" + err.path1().string() + "', " + string(err.what()) + '!');
        exit(1);
    }
    exit(0);
}

vector<_GXPM_conflict_t> mergable(vector<string> repositories, bool dry, bool &stat)
{
    const vector<_GXPM_repository_t> repositories_data = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_metadata_conflict_t> merged_metadatas;
    vector<_GXPM_conflict_t> conflicts;

    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    for (string repo : repositories)
    {
        bool found = false;
        for (_GXPM_repository_t r : repositories_data)
        {
            if (r.name == repo)
            {
                found = true;
                const string path = r.path + "/metadata.riff";
                const vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(path, stat);
                if (!stat)
                {
                    print_error("failed to open metadata file of '" + repo + "' repository!");
                    exit(1);
                }
                const vector<_GXPM_dependecies_t> deps = read_dependecies_chunks(path, stat);
                if (!stat)
                {
                    print_error("failed to open metadata file of '" + repo + "' repository!");
                    exit(1);
                }

                for (_GXPM_metadata_t m : metadatas)
                {
                    bool conflicted = false;
                    for (_GXPM_metadata_conflict_t mm : merged_metadatas)
                    {
                        if (m.libname == mm.libname || m.pathname == mm.pathname)
                        {
                            _GXPM_conflict_t conflict = {repo, m.libname, mm.repository};
                            conflicts.push_back(conflict);
                            conflicted = true;
                            if (verbose)
                                cout << "['" << m.libname << "' library from '" << repo << "' conflicted to '" + mm.repository + "' repository]\n";
                        }
                    }
                    if (!conflicted)
                    {
                        _GXPM_metadata_conflict_t mc;
                        mc.dep_id = m.dep_id;
                        mc.description = m.description;
                        mc.libname = m.libname;
                        mc.mainpoint_relative = m.mainpoint_relative;
                        mc.pathname = m.pathname;
                        mc.version = m.version;
                        mc.repository = repo;
                        merged_metadatas.push_back(mc);
                    }
                }
            }
        }
        if (!found)
        {
            print_error("invalid repository '" + repo + "'!");
            exit(1);
        }
    }

    if (conflicts.size())
    {
        string list;
        for (_GXPM_conflict_t c : conflicts)
        {
            list += ("[" + c.libname + "] (from '" + c.repository + "') => (to '" + c.repository_to + "')\n");
        }
        print_error("there is conflict with " + to_string(conflicts.size()) + " items!\nlist:\n" + list);
        if (!dry)
            exit(1);
        return conflicts;
    }
    else if (verbose)
        cout << "[no conflicts, its mergable]\n";

    if (!dry)
        exit(1);
    return conflicts;
}

void merge(vector<string> repositories, _GXPM_repository_t output)
{
    bool stat;

    vector<_GXPM_repository_t> repositories_data = read_repositories_chunk(REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to read repositories data file!");
        exit(1);
    }

    vector<_GXPM_metadata_t> merged_metadatas;
    vector<_GXPM_dependecies_t> merged_dependecies;
    const string metafile_path = output.path + "/metadata.riff";
    uint32_t libraries_count = 0;
    char last_dep_id[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t lvl = 0;

    if (!mergable(repositories, true, stat).size())
        exit(1);

    for (uint32_t i = 0; i < repositories.size(); i++)
    {
        const string repo = repositories[i];
        for (_GXPM_repository_t r : repositories_data)
        {
            if (r.name == repo)
            {
                const string path = r.path + "/metadata.riff";
                vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(path, stat);
                if (!stat)
                {
                    print_error("failed to open metadata file of '" + repo + "' repository!");
                    exit(1);
                }
                vector<_GXPM_dependecies_t> deps = read_dependecies_chunks(path, stat);
                if (!stat)
                {
                    print_error("failed to open metadata file of '" + repo + "' repository!");
                    exit(1);
                }

                for (_GXPM_metadata_t &m : metadatas)
                {
                    for (_GXPM_dependecies_t d : deps)
                    {
                        if (d.id == m.dep_id)
                        {
                            if (!last_dep_id[0])
                            {
                                last_dep_id[0] = 1;
                            }
                            else
                            {
                                if (!create_8byte_id(last_dep_id, lvl))
                                {
                                    print_error("out of creating unique id for dependecies!");
                                    exit(1);
                                };
                            }

                            m.dep_id = last_dep_id;
                            d.id = m.dep_id;
                            merged_metadatas.push_back(m);
                            merged_dependecies.push_back(d);
                            libraries_count++;
                        }
                    }
                }
            }
        }
    }

    try
    {
        fs::create_directory(output.path);
        for (uint32_t i = 0; i < repositories.size(); i++)
        {
            for (string repo : repositories)
            {
                for (_GXPM_repository_t r : repositories_data)
                {
                    if (repo == r.name)
                    {
                        for (const auto &i : fs::directory_iterator(r.path))
                        {
                            if (i.is_directory())
                            {

                                for (_GXPM_metadata_t m : merged_metadatas)
                                {
                                    const fs::path p = m.pathname;
                                    if (absolute_path(i.path().string()) == absolute_path(p.string()))
                                    {
                                        fs::copy(absolute_path(i.path().string()), absolute_path(output.path));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        output.libraries_count = libraries_count;

        vector<_GXPM_metadata_chunk_t> metadata_chunks;
        for (uint32_t i = 0; i < merged_metadatas.size(); i++)
        {
            metadata_chunks.push_back(to_metadata_chunk(merged_metadatas[i]));
        }
        write_initial_chunk(metafile_path, true, stat);
        if (!stat)
        {
            print_error("failed to append metadata file!");
            if (fs::exists(output.path))
                fs::remove(output.path);
            exit(1);
        }
        write_libs_chunks(metadata_chunks, merged_dependecies, metafile_path, stat);
        if (!stat)
        {
            print_error("failed to write into metadata file!");
            if (fs::exists(output.path))
                fs::remove(output.path);
            exit(1);
        }
        vector<_GXPM_repository_chunk_t> rchunks;
        repositories_data.push_back(output);
        for (_GXPM_repository_t r : repositories_data)
        {
            rchunks.push_back(to_repository_chunk(r));
        }
        write_repos_chunks(rchunks, REPOSITORIES_PATH, stat);
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("filesystem failure, " + string(err.what()) + ", at path '" + err.path1().string() + (err.path2().string().length() ? "', to" + err.path2().string() : "'!"));
        exit(1);
    }
}
void lint(string repo)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    _GXPM_repository_t target_repo;
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    for (_GXPM_repository_t r : repos)
    {
        if (fs::exists(repo) && fs::is_directory(repo))
        {
            if (r.path == repo)
            {
                target_repo = r;
                break;
            }
        }
        else if (r.name == repo)
        {
            target_repo = r;
            break;
        }
    }

    if (!target_repo.name.length())
    {
        print_error("'" + repo + "' repository isnt valid!");
        exit(1);
    }

    bool repo_metadata_valid = validate_chunks(target_repo.path + "/metadata.riff", stat);
    if (!stat)
    {
        print_error("failed to open repository metadata file!");
        exit(1);
    }

    if (repo_metadata_valid)
        cout << "['" << repo << "' repository is valid]\n";
    else
        cout << "['" << repo << "' repository isnt valid!]\n";
    exit(0);
}

void create_mainpoint(string path, string output, string repository, bool ignore_dependecies, bool ignore_repository)
{
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        print_error("invalid path to create main point!");
        exit(1);
    }
    _GXL_ _linker_;
    fstream f;
    _GXL_config_t cnf;
    bool stat = false;

    if (fs::exists(output) && fs::is_directory(output))
    {
        print_error("invalid output path '" + output + "'!");
        exit(1);
    }
    if (!repository_exists(repository, repository, stat))
    {
        print_error("invalid repository for '" + repository + "'!");
        exit(1);
    }

    cnf.repository_check = !ignore_repository;
    cnf.dependecy_check = !ignore_dependecies;
    cnf.metadata_file_name = "metadata.riff";
    cnf.repository_path = repository;

    _linker_.init(read_files(path), cnf);
    if (!_linker_.merge())
    {
        print_error("linker errored!\nerrors:\n");
        print_linker_errors(_linker_.errors);
        exit(1);
    }
    f.open(path, ios::out | ios::trunc);
    if (!f.is_open())
    {
        print_error("failed to write into output file '" + output + "'!");
        exit(1);
    }
    f << _linker_.merged;
    f.close();
    exit(0);
}

void validate_repository(string repository)
{
    bool found = false;
    bool stat = false;
    _GXPM_repository_t repo = find_repository(repository, repository, found, stat);

    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    if (!found)
    {
        print_error("'" + repository + "' not found!");
        exit(1);
    }

    const string mpath = repo.path + "/metadata.riff";
    found = validate_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + mpath + "' metadata file!");
        exit(1);
    }
    if (!found)
    {
        print_error("'" + repository + "' has invalid metadata file!");
        exit(0);
    }

    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + mpath + "'!");
        exit(1);
    }
    vector<_GXPM_dependecies_t> dependecies = read_dependecies_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + mpath + "'!");
        exit(1);
    }

    for (_GXPM_metadata_t m : metadatas)
    {

        for (_GXPM_dependecies_t d : dependecies)
        {
            if (d.id == m.dep_id)
            {
                found = true;
                break;
            }
        }
        if (!found)
            cout << "['" << m.libname << "' library dependecy chunk not found!]\n";
    }

    exit(0);
}