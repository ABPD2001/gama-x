#ifndef CMDS_COMMON
#define CMDS_COMMON
#define GXPM_PATH "/etc/gxpm"
#define CONFIGURATION_PATH "/etc/gxpm/config.cnf"
#define REPOSITORIES_PATH "/etc/gxpm/repos.riff"
#define GLOBAL_REPOSITORY_PATH "/etc/gxpm/globals"
#define DEFAULT_CONFIG_FORMAT "metadata_filename=metadata.riff"
#include <string>
#include <iostream>
#include "../../utils/riff.hpp"
#include "../structures.hpp"

using std::cout;
using std::string;

void print_error(string text);
void _wrtie_config_(_GXPM_config_t config);
bool _validate_config_(string content);
bool create_8byte_id(char bytes[8], uint32_t &lvl);
bool repository_exists(string name, string path, bool &stat);
#endif