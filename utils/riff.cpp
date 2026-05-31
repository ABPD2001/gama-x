#include "./riff.hpp"

void write_initial_chunk(string filepath, bool append, bool &stat)
{
    fstream f;

    if (!fs::exists(filepath) && !append)
    {
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::trunc);
    _GXPM_chunk_header_t header = {{'I', 'N', 'I', 'T'}, 0};
    f.write((char *)&header, CHUNK_HEADER_SIZE);
    f.close();
}

void write_repos_chunks(vector<_GXPM_repository_chunk_t> repos, string filepath, bool &stat)
{
    fstream f;
    _GXPM_chunk_header_t *headers;
    headers = new _GXPM_chunk_header_t[repos.size()];

    if (!fs::exists(filepath))
    {
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!f.is_open())
    {
        stat = false;
        return;
    }

    for (uint32_t i = 0; i < repos.size(); i++)
    {
        _GXPM_chunk_header_t header = {{'M', 'E', 'T', 'A'}, sizeof(_GXPM_repository_chunk_t)};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < repos.size(); i++)
    {
        f.write((char *)&headers[i], CHUNK_HEADER_SIZE);
        f.write((char *)&repos[i], sizeof(_GXPM_repository_chunk_t));
    }

    f.close();
    stat = true;
}

void write_libs_chunks(vector<_GXPM_metadata_chunk_t> metadatas, vector<_GXPM_dependecies_t> dependecies, string filepath, bool &stat)
{
    fstream f;
    _GXPM_chunk_header_t *headers;
    headers = new _GXPM_chunk_header_t[metadatas.size()];

    if (!fs::exists(filepath))
    {
        stat = false;
        return;
    }

    f.open(filepath, ios::out | ios::binary | ios::trunc);
    if (!f.is_open())
    {
        stat = false;
        return;
    }

    for (uint32_t i = 0; i < metadatas.size(); i++)
    {
        _GXPM_chunk_header_t header = {{'M', 'E', 'T', 'A'}, sizeof(_GXPM_metadata_chunk_t)};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < metadatas.size(); i++)
    {
        f.write((char *)&headers[i], CHUNK_HEADER_SIZE);
        f.write((char *)&metadatas[i], sizeof(_GXPM_metadata_chunk_t));
    }

    headers = new _GXPM_chunk_header_t[dependecies.size()];

    for (uint32_t i = 0; i < dependecies.size(); i++)
    {
        _GXPM_chunk_header_t header = {{'D', 'E', 'P', 'S'}, sizeof(dependecies[i])};
        headers[i] = header;
    }
    for (uint32_t i = 0; i < dependecies.size(); i++)
    {
        char terminator = NULL;

        f.write((char *)&headers[i], CHUNK_HEADER_SIZE);
        f.write((char *)&dependecies[i].id, 8); // 8 bytes as id.

        for (uint32_t j = 0; j < dependecies[i].dependecies.size(); j++)
        {
            f.write((char *)&dependecies[i].dependecies[j], dependecies[i].dependecies[j].length());
        }

        f.write((char *)terminator, 1);
    }
    f.close();
    stat = true;
}

vector<_GXPM_repository_t> read_repositories_chunk(string filepath, bool &stat)
{
    vector<_GXPM_repository_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    _GXPM_repository_chunk_t temp_chunk;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (file_size - f.tellg() >= CHUNK_HEADER_SIZE)
    {
        f.read((char *)&temp_header, CHUNK_HEADER_SIZE);
        if (string(temp_header.id) == "INIT")
        {
            f.close();
            return output;
        }
        if (string(temp_header.id) == "META")
        {
            f.read((char *)&temp_chunk, sizeof(_GXPM_repository_t));
            _GXPM_repository_t repo = {temp_chunk.description, temp_chunk.pathname, temp_chunk.name, temp_chunk.libraries_count};
            output.push_back(repo);
        }
        else
        {
            stat = false;
            return output;
        }
    }
    f.close();

    return output;
}

vector<_GXPM_metadata_t> read_metadata_chunks(string filepath, bool &stat)
{
    vector<_GXPM_metadata_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    _GXPM_metadata_chunk_t temp_chunk;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (file_size - f.tellg() >= CHUNK_HEADER_SIZE)
    {
        f.read((char *)&temp_header, CHUNK_HEADER_SIZE);
        if (string(temp_header.id) == "INIT")
        {
            f.close();
            return output;
        }
        if (string(temp_header.id) == "META")
        {
            f.read((char *)&temp_chunk, sizeof(_GXPM_metadata_chunk_t));

            _GXPM_metadata_t metadata = {temp_chunk.description, temp_chunk.mainpoint_relative, temp_chunk.pathname, temp_chunk.libname, temp_chunk.version, temp_chunk.dep_id};
            output.push_back(metadata);
        }
        else
        {
            stat = false;
            f.close();
            return output;
        }
    }
    f.close();

    return output;
}

vector<_GXPM_dependecies_t> read_dependecies_chunks(string filepath, bool &stat)
{
    vector<_GXPM_dependecies_t> output;
    fstream f;

    if (!fs::exists(filepath))
    {
        stat = false;
        return output;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return output;
    }

    _GXPM_chunk_header_t temp_header;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (file_size - f.tellg() >= CHUNK_HEADER_SIZE)
    {
        f.read((char *)&temp_header, CHUNK_HEADER_SIZE);
        if (string(temp_header.id) == "INIT")
        {
            f.close();
            return output;
        }
        if (string(temp_header.id) == "DEPS")
        {
            _GXPM_dependecies_t dependecies;
            string dependecies_txt;
            char ch;

            f.read((char *)&dependecies.id, 8); // read 8 bytes as id of dependecies.
            while (ch || dependecies_txt.empty())
            {
                f.read(&ch, 1);
                dependecies_txt += ch;
            }

            if (dependecies_txt.length())
                dependecies_txt = dependecies_txt.substr(0, dependecies_txt.length() - 1);
            dependecies.dependecies = split(dependecies_txt, ',');
        }
        else
        {
            stat = false;
            f.close();
            return output;
        }
    }
    f.close();

    return output;
}

bool validate_chunks(string filepath, bool &stat)
{
    fstream f;
    char empties[7] = {'\r', '\n', ' ', '\t', '\a'};

    if (!fs::exists(filepath))
    {
        stat = false;
        return false;
    }
    f.open(filepath, ios::in | ios::binary | ios::ate);
    if (!f.is_open())
    {
        stat = false;
        return false;
    }

    _GXPM_chunk_header_t temp_header;
    const uint32_t file_size = f.tellg();
    f.seekg(0);

    while (file_size - f.tellg() >= CHUNK_HEADER_SIZE)
    {
        f.read((char *)&temp_header, CHUNK_HEADER_SIZE);

        if (string(temp_header.id) == "META")
        {
            _GXPM_metadata_chunk_t temp_chunk;
            f.read((char *)&temp_chunk, sizeof(_GXPM_metadata_chunk_t));
            if (!filter(temp_chunk.description, empties, 7).size() || !fs::exists(temp_chunk.mainpoint_relative) || !fs::exists(temp_chunk.pathname) || !fs::is_directory(temp_chunk.pathname) || !filter(temp_chunk.libname, empties, 7).size() || !is_valid_version(temp_chunk.version))
                return false;
        }
        else if (string(temp_header.id) == "DEPS")
        {
            vector<string> deps;
            string text;
            string id;
            char ch;

            f.read((char *)&id, 8);
            if (!filter(string(id), empties, 7).length())
                return false;
            while (ch || text.empty())
            {
                f.read(&ch, 1);
                text += ch;
            }
            deps = split(text, ',');
            for (string d : deps)
            {
                if (!filter(d, empties, 7).length())
                    return false;
            }
        }
        else if (string(temp_header.id) == "REPO")
        {
        }
        else if (string(temp_header.id) == "INIT")
        {
            if (temp_header.size_b)
                return false;
            if (file_size - f.tellg() >= CHUNK_HEADER_SIZE)
                return false;
            return true;
        }
        else
        {
            stat = true;
            return false;
        }
    }
    f.close();

    return true;
}

_GXPM_metadata_chunk_t to_metadata_chunk(_GXPM_metadata_t metadata)
{
    _GXPM_metadata_chunk_t output;

    for (uint32_t i = 0; i < min(metadata.description.length(), 150); i++)
    {
        output.description[i] = metadata.description[i];
    }
    for (uint32_t i = 0; i < min(metadata.mainpoint_relative.length(), 150); i++)
    {
        output.mainpoint_relative[i] = metadata.mainpoint_relative[i];
    }
    for (uint32_t i = 0; i < min(metadata.libname.length(), 150); i++)
    {
        output.libname[i] = metadata.libname[i];
    }
    for (uint32_t i = 0; i < min(metadata.version.length(), 150); i++)
    {
        output.version[i] = metadata.version[i];
    }
    for (uint32_t i = 0; i < min(metadata.pathname.length(), 150); i++)
    {
        output.pathname[i] = metadata.pathname[i];
    }

    return output;
}

_GXPM_repository_chunk_t to_repository_chunk(_GXPM_repository_t repo)
{
    _GXPM_repository_chunk_t output;

    for (uint32_t i = 0; i < repo.description.length(); i++)
    {
        output.description[i] = repo.description[i];
    }
    for (uint32_t i = 0; i < repo.path.length(); i++)
    {
        output.pathname[i] = repo.path[i];
    }
    for (uint32_t i = 0; i < repo.name.length(); i++)
    {
        output.name[i] = repo.name[i];
    }

    return output;
}