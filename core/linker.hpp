#ifndef LINKER_HPP
#define LINKER_HPP
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#define META_CHUNK_SIZE 280
#define CHUNK_HEADER_SIZE 8

using std::cout;
using std::fstream;
using std::getline;
using std::ios;
using std::string;
using std::uint32_t;
using std::vector;

struct _GXL_error_t
{
    string file;
    string message;
    string type;
    string cause;
    uint64_t index;
};

struct _GXL_file_t
{
    string content;
    string name;
};

struct _GXL_config_t
{
    string metadata_file_name;
    bool repository_check;
    bool dependecy_check;
};

struct _GXL_library_metadata_chunk_header_t // 8 bytes.
{
    char id[4];      // META for metadata chunk, DEPS for dependecies
    uint32_t size_b; // size by bytes.
};

// for dependecies, for id, 8 byte is allocated, for dependecies names, size is dynamic.

struct _GXL_library_metadata_chunk_t // 282 Bytes => 288 Bytes.
{
    char description[150];       // just for package manager.
    char mainpoint_relative[48]; // xxxx/xxxxx .
    char pathname[32];           // name where stores with it on directory.
    char libname[32];            // name where called with '.import'.
    char version[12];            // just for package manager, XX.XXX.XXXXX .
    char dep_id[8];              // id of dependecies chunk.
    char padding[6];             // padding for memory alignment.
};

struct _GXL_library_dependecy_t
{ // a user-firendly version structure from dependecy chunk.
    const vector<string> dependecies;
    string id;
};

struct _GXL_library_metadata_t // a user-friendly version structure from metadata chunk.
{
    vector<string> dependecies;
    string description;        // just for package manager.
    string mainpoint_relative; // xxxx/xxxxx .
    string pathname;           // name where stores with it on directory.
    string libname;            // name where called with '.import'.
    string version;            // just for package manager, XX.XXX.XXXXX .
};

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
    vector<_GXL_error_t> errors;

    _GXL_();
    _GXL_(vector<_GXL_file_t> files, _GXL_config_t config, string repository);
    bool merge();
};
#endif