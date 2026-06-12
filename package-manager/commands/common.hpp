#ifndef CMDS_COMMON
#define CMDS_COMMON
#define GXPM_PATH "/etc/gxpm"

#define GLOBAL_REPOSITORY_PATH "/etc/gxpm/globals"
#include <string>
#include <iostream>
#include "../../utils/riff.hpp"
#include "../structures.hpp"

using std::cout;
using std::getenv;
using std::string;
using std::system;

extern bool verbose;

extern string _HOME_;
extern string GXPM_LOCAL_PATH;
extern string REPOSITORIES_PATH;

void print_error(string text);
bool create_8byte_id(char bytes[8], uint32_t &lvl);
bool repository_exists(string name, string path, bool &stat);
bool library_exists(string name, string path, string repository, bool &stat);
vector<_GXL_file_t> read_files(string path);
_GXPM_repository_t find_repository(string name, string path, bool &found, bool &stat);
void print_linker_errors(vector<_GXLT_error_t> errors);

inline string absolute_path(string path)
{
    const fs::path p = path;
    return fs::absolute(p).string();
}
#endif