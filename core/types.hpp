#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP
#include <string>
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

#include "../user/core.hpp"

using std::string;
using std::vector;

using std::cout;
using std::fstream;
using std::ios;

using std::getline;
using std::to_string;

using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

using _GXA_snippet_t = vector<logictype_t>;

namespace fs = std::filesystem;

struct _GXLT_error_t
{
    string file;
    string message;
    string cause;
    string type;
    uint32_t line;
};

struct _GXA_label_t
{
    string text;
    string name;
    uint32_t line_idx;
};

struct _GXA_external_argument_t
{
    string name;
    string value;
};
struct _GXA_external_t
{
    string content;
    string name;
};

struct _GXA_config_t
{
    vector<string> registers_name;
    vector<string> special_registers_name;
    vector<logictype_t> register_values;
};

struct _GXL_file_t
{
    string content;
    string name;
};

struct _GXL_config_t
{
    string repository_path;
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

#endif