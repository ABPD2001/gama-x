#ifndef GXPM_structures_HPP
#define GXPM_structures_HPP
#include "../core/types.hpp"

struct _GXPM_config_t
{
    string metadata_filename;
};

struct _GXPM_conflict_t
{
    string libname;
    string repository;
    string repository_to;
};

struct _GXPM_metadata_conflict_t : _GXPM_metadata_t
{
    string repository;
};
#endif