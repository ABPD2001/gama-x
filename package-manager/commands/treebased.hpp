#ifndef TREEBASED_CMD_HPP
#define TREEBASED_CMD_HPP
#include "../../core/types.hpp"
#include "../../utils/riff.hpp"
#include "./common.hpp"

extern _GXPM_config_t configuration;

void add(string name, string path, string repository, bool is_repo);
void remove(string name, string path, string repository, string id, bool is_repo);
void edit(string name, string path, string repository, string description, bool is_repo);
#endif