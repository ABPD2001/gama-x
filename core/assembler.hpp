#ifndef _GXA__HPP
#define _GXA__HPP
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

#include "../user/core.hpp"

using std::cout;
using std::fstream;
using std::getline;
using std::ios;
using std::max;
using std::min;
using std::string;

using std::abs;
using std::ceill;
using std::cosl;
using std::floorl;
using std::logl;
using std::pow;
using std::roundl;
using std::sinhl;
using std::sinl;
using std::sqrt;
using std::tanl;

using std::to_string;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;

namespace fs = std::filesystem;

struct _GXA_label_t
{
    string text;
    string name;
    uint32_t line_idx;
};

struct _GXA_error_t
{
    string message;
    string cause;
    string type;
    uint32_t line;
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
    vector<_GXA_external_t> externs;
    vector<string> register_names;
    vector<logictype_t> register_values;
    string argular_content;
};

class _GXA_
{
    _GXA_config_t config;
    vector<_GXA_snippet_t> output;
    vector<_GXA_label_t> labels;

    vector<logictype_t> vir_regs = {0, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<logictype_t> vir_stack;
    vector<string> vir_regs_names = {"x", "y", "t", "r0", "r1", "r2", "r3", "r4", "r5"};

    vector<_GXA_external_argument_t> external_arguments;
    vector<_GXA_external_t> externals;

    string input;
    string startLabel;

    uint64_t vir_sp = 0; // --- //

    vector<_GXA_snippet_t> _transpile_(string content, uint64_t line_idx);
    vector<_GXA_label_t> _slice_labels_(string content);

    vector<_GXA_external_argument_t> _parse_argular_(string path, uint32_t index);
    void _open_extern_(string path, string name, uint32_t index);

    void _pre_processors_(string &content);
    void _check_errors_(string content, uint32_t index);
    void _GXA_::_check_pre_processors_errors_();

public:
    vector<_GXA_error_t> errors;
    uint8_t status = 0;

    _GXA_(string content, _GXA_config_t config);
    _GXA_();
    ~_GXA_();
    vector<_GXA_snippet_t> compile();
};
#endif
