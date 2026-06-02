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
    uint64_t line;
};

struct _GXA_label_t
{
    string text;
    string name;
    uint64_t line_idx;
};

struct _GXA_external_argument_t
{
    string value;
    string name;
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

struct _GXPM_repository_t // a user-freindly version structure from repository.
{
    string description; // just for package manager.
    string path;
    string name;
    uint32_t libraries_count; // count of libraries inside repository.
    // string owner_user_id; // 16 bytes as id of owner user.
    // bool access[4];       // bytes --> <read,write,delete,add>.
};

struct _GXPM_metadata_t
{                              // a user-friendly version structure from metadata.
    string description;        // just for package manager.
    string mainpoint_relative; // xxxx/xxxxx .
    string pathname;           // name where stores with it on directory.
    string libname;            // name where called with '.import'.
    string version;            // just for package manager, XX.XXX.XXXXX .
    string dep_id;             // id of dependecies chunk.
};
struct _GXPM_dependecies_t
{ // a user-firendly version structure from dependecy chunk.
    vector<string> dependecies;
    string id;
};

struct _GXPM_chunk_header_t // 8 bytes.
{
    char id[4];      // META for metadata chunk, DEPS for dependecies
    uint32_t size_b; // size by bytes.
};

struct _GXPM_repository_chunk_t // 244 Bytes.
{
    char description[152]; // just for package manager.
    char pathname[56];     // path of repository.
    char name[32];         // name of repository.
    // char owner_user_id[16]; // 16 bytes as id of owner user.
    // bool access[4];         // bytes --> <read,write,delete,add>.
    uint32_t libraries_count; // count of libraries at repository.
};

struct _GXPM_metadata_chunk_t // 288 Bytes
{
    char description[152];       // just for package manager.
    char mainpoint_relative[49]; // xxxx/xxxxx .
    char pathname[33];           // name where stores with it on directory.
    char libname[33];            // name where called with '.import'.
    char version[13];            // just for package manager, XX.XXX.XXXXX .
    char dep_id[8];              // id of dependecies chunk.
};

// for dependecies, for id, 8 byte is allocated, for dependecies names, size is dynamic.
#endif