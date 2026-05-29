#include "./riff.hpp"

void write_chunks(vector<_GXPM_metadata_chunk_t> metadatas, vector<_GXPM_dependecies_t> dependecies, string filepath, bool &stat)
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
        _GXPM_chunk_header_t header = {'META', sizeof(_GXPM_metadata_chunk_t)};
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
        _GXPM_chunk_header_t header = {'DEPS', sizeof(dependecies[i])};
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
        if (string(temp_header.id) == "META")
        {
            f.read((char *)&temp_chunk, sizeof(_GXPM_metadata_chunk_t));
            _GXPM_metadata_t metadata = {temp_chunk.description, temp_chunk.mainpoint_relative, temp_chunk.pathname, temp_chunk.libname, temp_chunk.version, temp_chunk.dep_id};
            output.push_back(metadata);
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
        else
        {
            stat = true;
            return false;
        }
    }
    f.close();

    return true;
}