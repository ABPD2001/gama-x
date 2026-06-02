#ifndef BASE_CMDS_HPP
#define BASE_CMDS_HPP
#include "../../utils/riff.hpp"
#include "../../utils/string.hpp"
#include "./common.hpp"

extern _GXPM_config_t configuration;
extern bool verbose;

void setup();
void initialize(string path);
void reset(bool rage);
void configure(string filepath);
vector<_GXPM_conflict_t> mergable(vector<string> repositories, bool dry, bool &stat);
void merge(vector<string> repositories, _GXPM_repository_t output);
void lint(string repo);
#endif