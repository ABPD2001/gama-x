#ifndef LINKER_HPP
#define LINKER_HPP
#include "./types.hpp"

#define META_CHUNK_SIZE 280
#define CHUNK_HEADER_SIZE 8

class _GXL_
{
    vector<_GXL_file_t> files;
    vector<string> requested_libraries;

    _GXL_config_t config;

    string merge_preprocessor;
    string merge_instruction;
    string repository = "/usr/lib/gamax";

    void _merge_files_();
    void _add_libraries_();

public:
    string merged;
    vector<_GXLT_error_t> errors;

    _GXL_();
    _GXL_(vector<_GXL_file_t> files, _GXL_config_t config);
    void init(vector<_GXL_file_t> files, _GXL_config_t config);
    bool merge();
};
#endif