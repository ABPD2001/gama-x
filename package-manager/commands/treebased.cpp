#include "./treebased.hpp"

void add_repo(string name, string path, string description)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_chunk_t> chunks;
    fstream f;
    _GXPM_repository_t repo;
    string content;
    char ch;

    if (!fs::exists(description) || !fs::is_regular_file(description))
    {
        print_error("invalid description file '" + description + "'!");
        exit(1);
    }
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    vector<_GXPM_metadata_t> repo_metadatas = read_metadata_chunks(absolute_path(path) + "/metadata.riff", stat);
    if (!stat)
    {
        print_error("failed to open '" + path + "' repository metadata file!");
        exit(1);
    }
    repo.name = name;
    repo.libraries_count = repo_metadatas.size();
    repo.path = absolute_path(path);

    f.open(description, ios::in);
    while (f.read(&ch, 1))
    {
        content += ch;
    }
    f.close();

    repo.description = content;
    repos.push_back(repo);

    for (uint32_t i = 0; i < repos.size(); i++)
    {
        chunks.push_back(to_repository_chunk(repos[i]));
    }

    write_repos_chunks(chunks, REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to write into repositories data file!");
        exit(1);
    }
    exit(0);
}

void add_lib(string deps_txt, string repository, string version, string mainpoint_relative, string name, string description, string path, string target_repo)
{
    fstream f;
    string content;
    vector<string> deps;
    uint32_t lvl = 0;
    char id[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool stat = false;
    bool found = false;

    if (deps_txt.length())
        deps = split(deps_txt, ',');

    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_chunk_t> rchunks;
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    _GXPM_repository_t r = find_repository(repository, repository, found, stat);
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    if (!found)
    {
        print_error("'" + repository + "' repository not found!");
        exit(1);
    }
    if (!fs::exists(description))
    {
        print_error("invalid decription file '" + description + "'!");
        exit(1);
    }
    f.open(description, ios::in);
    while (f.read(&id[0], 1))
    {
        content += id[0];
    }
    id[0] = 0;

    const string mpath = r.path + "/metadata.riff";

    vector<_GXPM_metadata_chunk_t> mchunks;
    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open repository '" + mpath + "' metadata file!");
        exit(1);
    }

    vector<_GXPM_dependecies_t> lib_deps = read_dependecies_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open repository '" + mpath + "' metadata file!");
        exit(1);
    }
    _GXPM_metadata_t metadata;
    _GXPM_dependecies_t dependecies;

    for (_GXPM_dependecies_t &d : lib_deps)
    {
        if (!create_8byte_id(id, lvl))
        {
            print_error("out of unique id creation for dependecies!");
            exit(1);
        }
        d.id = id;
    }

    metadata.libname = name;
    metadata.pathname = path;
    metadata.version = version;
    metadata.mainpoint_relative = mainpoint_relative;
    metadata.description = content;

    if (deps_txt.length())
    {
        create_8byte_id(id, lvl);
        metadata.dep_id = id;
        dependecies.dependecies = deps;
        dependecies.id = id;

        lib_deps.push_back(dependecies);
    }
    else
        metadata.dep_id = "";

    metadatas.push_back(metadata);
    for (_GXPM_repository_t &rep : repos)
    {
        if (rep.name == r.name)
            rep.libraries_count++;
        rchunks.push_back(to_repository_chunk(rep));
    }

    for (_GXPM_metadata_t m : metadatas)
    {
        mchunks.push_back(to_metadata_chunk(m));
    }
    write_libs_chunks(mchunks, lib_deps, mpath, stat);
    if (!stat)
    {
        print_error("failed to write into '" + repository + "' repository metadata file!");
        exit(1);
    }
    write_repos_chunks(rchunks, REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to write into repositories data file!");
        exit(1);
    }

    exit(0);
}

void remove_repo(string repo)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_chunk_t> chunks;

    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    for (_GXPM_repository_t r : repos)
    {
        fs::path p = r.path;

        if (r.name == repo)
        {
            if (verbose)
                cout << "['" << repo << "' at '" << r.path << "' filtered]\n";
            continue;
        }
        else if ((fs::exists(p) && fs::is_regular_file(p)) && r.path == absolute_path(p.string()))
        {
            if (verbose)
                cout << "['" << repo << "' at '" << r.path << "' filtered]\n";

            continue;
        }
        chunks.push_back(to_repository_chunk(r));
    }

    write_repos_chunks(chunks, REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to write into repositories data file!");
        exit(1);
    }
    exit(0);
}

void remove_lib(string lib, string repository)
{
    string path;
    bool stat = false;
    if (!fs::exists(repository))
    {
        vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
        for (_GXPM_repository_t r : repos)
        {
            if (r.name == repository)
                path = r.path + "/metadata.riff";
        }
    }

    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(path, stat);
    vector<_GXPM_metadata_chunk_t> mchunks;
    vector<_GXPM_dependecies_t> dchunks;
    _GXPM_metadata_t tlib;
    if (!stat)
    {
        print_error("failed to open repository '" + repository + "' metadata file!");
        exit(1);
    }
    vector<_GXPM_dependecies_t> deps = read_dependecies_chunks(path, stat);
    if (!stat)
    {
        print_error("failed to open repository '" + repository + "' metadata file!");
        exit(1);
    }

    for (_GXPM_metadata_t m : metadatas)
    {
        if (fs::exists(lib) && fs::is_directory(lib) && absolute_path(lib) == m.pathname)
        {
            if (verbose)
                cout << "['" << lib << "' library filtered from '" << repository << "' repository]\n";
            continue;
        }
        else if (m.libname == lib)
        {
            if (verbose)
                cout << "['" << lib << "' library filtered from '" << repository << "' repository]\n";
            tlib = m;
            continue;
        }
        mchunks.push_back(to_metadata_chunk(m));
    }
    for (_GXPM_dependecies_t d : deps)
    {
        if (d.id == tlib.dep_id)
            continue;
        dchunks.push_back(d);
    }

    write_libs_chunks(mchunks, dchunks, path, stat);
    if (!stat)
    {
        print_error("failed to write into '" + repository + "' repository metadata!");
        exit(1);
    }
    exit(0);
}

void edit_repo(_GXPM_repository_t edits, string repo)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_chunk_t> chunks;
    const fs::path p = repo;
    bool found = false;
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    for (_GXPM_repository_t &r : repos)
    {
        if ((fs::exists(p) && absolute_path(p.string()) == r.path) || repo == r.name)
        {
            if (edits.description.length())
                r.description = edits.description;
            if (edits.name.length())
                r.name = edits.name;
            if (edits.path.length())
                r.path = edits.path;
            if (edits.libraries_count != string::npos)
                r.path = edits.libraries_count;
            found = true;
            break;
        }
    }
    if (!found)
    {
        print_error("repository '" + repo + "' not found!");
        exit(1);
    }
    for (_GXPM_repository_t r : repos)
    {
        chunks.push_back(to_repository_chunk(r));
    }
    write_repos_chunks(chunks, REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to write into repositories data file!");
        exit(1);
    }
    exit(0);
}

void edit_lib(_GXPM_metadata_t edits, string lib, string repository)
{
    bool stat = false;
    bool found = false;
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
    const string path = repo.path + "/metadata.riff";
    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(path, stat);
    if (!stat)
    {
        print_error("failed to '" + path + "'data file!");
        exit(1);
    }
    vector<_GXPM_dependecies_t> deps = read_dependecies_chunks(path, stat);
    if (!stat)
    {
        print_error("failed to '" + path + "'data file!");
        exit(1);
    }

    vector<_GXPM_metadata_chunk_t> mchunks;
    const fs::path p = lib;

    for (_GXPM_metadata_t &m : metadatas)
    {
        if ((fs::exists(p) && absolute_path(p.string()) == m.pathname) || m.libname == lib)
        {
            if (edits.version.length())
                m.version = edits.version;
            if (edits.mainpoint_relative.length())
                m.mainpoint_relative = edits.mainpoint_relative;
            if (edits.libname.length())
                m.libname = edits.libname;
            if (edits.pathname.length())
                m.pathname = edits.pathname;
            if (edits.description.length())
                m.description = edits.description;
            found = true;
            break;
        }
    }
    if (!found)
    {
        print_error("library '" + lib + "' at '" + repository + "' repository not found!");
        exit(1);
    }
    for (_GXPM_metadata_t m : metadatas)
    {
        mchunks.push_back(to_metadata_chunk(m));
    }
    write_libs_chunks(mchunks, deps, path, stat);
    if (!stat)
    {
        print_error("failed to write into '" + repository + "' repository metadata file!");
        exit(1);
    }
    exit(0);
}

void copy_repos(vector<string> repositories, string to_path)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_t> target_repos;

    for (string rp : repositories)
    {
        const fs::path p = rp;
        bool stat = false;
        const bool exists = repository_exists(rp, rp, stat);
        if (!stat)
        {
            print_error("failed to open repositories data file!");
            exit(1);
        }
        if (!exists)
        {
            print_error("'" + rp + "' repository not found!");
            exit(1);
        }
        for (_GXPM_repository_t r : repos)
        {
            if ((fs::exists(rp) && (absolute_path(p.string()) == r.path)) || r.name == rp)
            {
                target_repos.push_back(r);
            }
        }
    }
    try
    {
        for (_GXPM_repository_t r : target_repos)
        {
            if (!fs::exists(to_path + "/" + r.name))
            {
                fs::create_directory(to_path + "/" + r.name);
            }
            else if (!fs::is_directory(to_path + "/" + r.name))
            {
                print_error("invalid target path '" + to_path + "'!");
                exit(1);
            }
            if (verbose)
                cout << "['" << r.path << "' copied into '" << to_path << "']\n";
            fs::copy(r.path, to_path + "/" + r.name, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to copy '" + err.path1().string() + "' to '" + to_path + "'!");
        exit(1);
    }

    exit(0);
}

void copy_libs(vector<string> libs, string repository, string target_repo)
{
    bool stat = false;
    const vector<_GXPM_repository_t> repositories = read_repositories_chunk(REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }

    _GXPM_repository_t from_repo;
    _GXPM_repository_t to_repo;
    const fs::path pf = repository;
    const fs::path pt = target_repo;

    for (_GXPM_repository_t r : repositories)
    {
        if (from_repo.name.length() && to_repo.name.length())
            break;
        if ((fs::exists(pf) && absolute_path(pf.string()) == r.path) || r.name == repository)
        {
            from_repo = r;
        }
        if ((fs::exists(pt) && absolute_path(pt.string()) == r.path) || r.name == repository)
        {
            to_repo = r;
        }
    }

    const string fpath = from_repo.path + "/metadata.riff";
    const string tpath = to_repo.path + "/metadata.riff";
    const vector<_GXPM_metadata_t> metadatas_from = read_metadata_chunks(fpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + repository + "' repository metadata file!");
    }
    const vector<_GXPM_metadata_t> metadatas_to = read_metadata_chunks(tpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + target_repo + "' repository metadata file!");
    }
    vector<_GXPM_dependecies_t> dependecies_from = read_dependecies_chunks(fpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + repository + "' repository metadata file!");
    }
    vector<_GXPM_dependecies_t> dependecies_to = read_dependecies_chunks(tpath, stat);
    if (!stat)
    {
        print_error("failed to open '" + target_repo + "' repository metadata file!");
    }

    vector<_GXPM_conflict_t> conflicts;
    vector<_GXPM_metadata_t> target_libs;
    vector<_GXPM_dependecies_t> target_libs_deps;

    for (string l : libs)
    {
        const fs::path p = l;
        for (_GXPM_metadata_t m : metadatas_from)
        {
            if ((fs::exists(p) && absolute_path(p.string()) == m.pathname) || l == m.libname)
            {
                target_libs.push_back(m);

                for (_GXPM_dependecies_t d : dependecies_from)
                {
                    if (d.id == m.dep_id)
                        target_libs_deps.push_back(d);
                }
                break;
            }
        }
    }

    for (_GXPM_metadata_t t : target_libs)
    {
        for (_GXPM_metadata_t m : metadatas_to)
        {
            if (t.libname == m.libname || t.pathname == m.pathname)
            {
                _GXPM_conflict_t c = {t.libname, repository, target_repo};

                conflicts.push_back(c);
                if (verbose)
                    cout << "['" << m.libname << "' library from '" << repository << "' conflicted to '" + target_repo + "' repository]\n";
            }
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
        exit(1);
    }

    vector<_GXPM_metadata_chunk_t> mchunks;
    vector<_GXPM_dependecies_t> dchunks;
    char id[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t lvl = 0;

    for (_GXPM_metadata_t m : metadatas_to)
    {
        if (!create_8byte_id(id, lvl))
        {
            print_error("unique id creation out of range!");
            exit(1);
        }
        for (_GXPM_dependecies_t &d : dependecies_to)
        {
            if (d.id == m.dep_id)
            {
                d.id = id;
                dchunks.push_back(d);
            }
        }
        m.dep_id = id;
        mchunks.push_back(to_metadata_chunk(m));
    }

    for (_GXPM_metadata_t t : target_libs)
    {
        if (!create_8byte_id(id, lvl))
        {
            print_error("unique id creation out of range!");
            exit(1);
        }
        for (_GXPM_dependecies_t &d : dependecies_from)
        {
            if (d.id == t.dep_id)
            {
                d.id = id;
                dchunks.push_back(d);
            }
        }
        t.dep_id = id;
        mchunks.push_back(to_metadata_chunk(t));
    }
    try
    {
        for (_GXPM_metadata_t l : target_libs)
        {
            fs::copy(l.pathname, to_repo.path);
        }
    }
    catch (const fs::filesystem_error &err)
    {
        print_error("failed to copy '" + err.path1().string() + "' into '" + err.path2().string() + " '!");
        exit(1);
    }

    write_libs_chunks(mchunks, dchunks, to_repo.path, stat);
    if (!stat)
    {
        print_error("failed to write into '" + to_repo.name + "' repository metadata file!");
        exit(1);
    }

    exit(0);
}

void list_repos(bool local_only)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    for (_GXPM_repository_t r : repos)
    {
        if (r.name == "global" && local_only)
            continue;
        cout << "['" << r.name << "', " << r.libraries_count << " librarires] (" << r.path << "): " << r.description << "\n";
    }
    exit(0);
}

void list_libs(string repository)
{
    bool stat = false;
    bool found = false;
    _GXPM_repository_t repo = find_repository(repository, repository, found, stat);

    if (!stat)
    {
        print_error("failed to open repositories data file!");
        exit(1);
    }
    if (!found)
    {
        print_error("'" + repository + "' does not exists!");
        exit(1);
    }

    vector<_GXPM_metadata_t> libs = read_metadata_chunks(repo.path + "/metadata.riff", stat);
    if (!stat)
    {
        print_error("failed to open '" + repository + "' repository metadata file!");
        exit(1);
    }
    vector<_GXPM_dependecies_t> deps = read_dependecies_chunks(repo.path + "/metadata.riff", stat);
    if (!stat)
    {
        print_error("failed to open '" + repository + "' repository metadata file!");
        exit(1);
    }

    for (_GXPM_metadata_t l : libs)
    {
        uint32_t deps_count = 0;
        for (_GXPM_dependecies_t d : deps)
        {
            if (d.id == l.dep_id)
            {
                deps_count = d.dependecies.size();
                break;
            }
        }
        cout << "[" << l.libname << "  V" << l.version << ", " << deps_count << " dependecies] (" << repo.name << " repository): " << l.pathname << ", " << l.mainpoint_relative << "\n";
    }
    exit(0);
}
