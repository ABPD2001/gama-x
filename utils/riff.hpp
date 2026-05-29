#ifndef RIFF_HPP
#define RIFF_HPP

#include "../core/types.hpp"
#define CHUNK_HEADER_SIZE 8

void write_libs_chunks(vector<_GXPM_metadata_t> metadatas, vector<_GXPM_dependecies_t> dependecies, string filepath, bool &stat);
void write_repos_chunks(vector<_GXPM_repository_chunk_t> repos, string filename, bool &stat);

vector<_GXPM_metadata_t> read_metadata_chunks(string filepath, bool &stat);
vector<_GXPM_dependecies_t> read_dependecies_chunks(string filepath, bool &stat);

vector<_GXPM_repository_chunk_t> read_repositories_chunk(string filepath, bool &stat);

bool validate_chunks(string filepath, bool &stat);
#endif