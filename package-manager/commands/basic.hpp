#ifndef BASE_CMDS_HPP
#define BASE_CMDS_HPP
#include "../../utils/riff.hpp"
#include "../../utils/string.hpp"
#include "../../core/linker.hpp"
#include "./common.hpp"

extern _GXPM_config_t configuration;
extern bool verbose;

void setup(bool local, bool local_ignore);
void initialize(string path);
void reset(bool rage);
vector<_GXPM_conflict_t> mergable(vector<string> repositories, bool dry, bool &stat);
void merge(vector<string> repositories, _GXPM_repository_t output);
void lint(string repo);
void create_mainpoint(string path, string output, string repository, bool ignore_dependecies, bool ignore_repository);
void validate_repository(string repository);
#endif