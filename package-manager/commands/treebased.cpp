#include "./treebased.hpp"

void add_repo(string name, string path, string description)
{
    bool stat = false;
    vector<_GXPM_repository_t> repos = read_repositories_chunk(REPOSITORIES_PATH, stat);
    vector<_GXPM_repository_chunk_t> chunks;
    fstream f;
    _GXPM_repository_t repo;
    string content;
    fs::path p = path;
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

    vector<_GXPM_metadata_t> repo_metadatas = read_metadata_chunks(path + "/" + (configuration.metadata_filename.length() ? configuration.metadata_filename : "metadata.riff"), stat);
    if (!stat)
    {
        print_error("failed to open '" + path + "' repository metadata file!");
        exit(1);
    }
    repo.name = name;
    repo.libraries_count = repo_metadatas.size();
    repo.path = fs::absolute(p).string();

    f.open(description, ios::out);
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

void add_lib(vector<string> deps, string repository, string version, string mainpoint_relative, string name, string description, string path, string target_repo)
{
    uint32_t lvl = 0;
    char id[8] = {0, 0, 0, 0, 0, 0, 0, 0};
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
                repository = r.path;
        }
    }
    const string mpath = path + "/" + (configuration.metadata_filename.length() ? configuration.metadata_filename : "metadata.riff");

    vector<_GXPM_metadata_chunk_t> mchunks;
    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open library '" + path + "' metadata file!");
        exit(1);
    }

    vector<_GXPM_dependecies_t> lib_deps = read_dependecies_chunks(mpath, stat);
    if (!stat)
    {
        print_error("failed to open library '" + path + "' metadata file!");
        exit(1);
    }
    _GXPM_metadata_t metadata;
    _GXPM_dependecies_t dependecies;

    for (_GXPM_dependecies_t d : lib_deps)
    {
        if (!create_8byte_id(id, lvl))
        {
            print_error("out of unique id creation for dependecies!");
            exit(1);
        }
        d.id = id;
    }
    create_8byte_id(id, lvl);

    metadata.libname = name;
    metadata.pathname = path;
    metadata.version = version;
    metadata.mainpoint_relative = mainpoint_relative;
    metadata.dep_id = id;

    dependecies.dependecies = deps;
    dependecies.id = id;

    lib_deps.push_back(dependecies);

    for (_GXPM_metadata_t m : metadatas)
    {
        mchunks.push_back(to_metadata_chunk(m));
    }
    mchunks.push_back(to_metadata_chunk(metadata));

    write_libs_chunks(mchunks, lib_deps, repository + "/" + (configuration.metadata_filename.length() ? configuration.metadata_filename : "metadata.riff"), stat);
    if (!stat)
    {
        print_error("failed to write into '" + path + "' library metadata file!");
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
        else if ((fs::exists(p) && fs::is_regular_file(p)) && r.path == fs::absolute(p))
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
                repository = r.path;
        }
    }
    vector<_GXPM_metadata_t> metadatas = read_metadata_chunks(repository + "/" + (configuration.metadata_filename.length() ? configuration.metadata_filename : "metadata.riff"), stat);
    vector<_GXPM_metadata_chunk_t> mchunks;
    if (!stat)
    {
        print_error("failed to open repository '" + repository + "' metadata file!");
        exit(1);
    }
    for (_GXPM_metadata_t m : metadatas)
    {
        if (fs::exists(lib) && fs::is_directory(lib) && fs::absolute(lib) == m.pathname)
        {
            if (verbose)
                cout << "['" << lib << "' library filtered from '" << repository << "' repository]\n";
            continue;
        }
        else if (m.libname == lib)
        {
            if (verbose)
                cout << "['" << lib << "' library filtered from '" << repository << "' repository]\n";
            continue;
        }
    }
}