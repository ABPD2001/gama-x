#ifndef TREEBASED_CMD_HPP
#define TREEBASED_CMD_HPP
#include "../../core/types.hpp"
#include "../../utils/riff.hpp"
#include "./common.hpp"

extern _GXPM_config_t configuration;
bool verbose;

void add_repo(string name, string path, string description);
void add_lib(vector<string> deps, string repository, string version, string mainpoint_relative, string name, string description, string path, string target_repo);

void remove_repo(string repo);
void remove_lib(string lib, string repository);

void edit_repo(_GXPM_repository_t edits, string repo);
void edit_lib(_GXPM_repository_t edits, string lib, string repository);
#endif