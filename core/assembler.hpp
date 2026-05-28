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

struct _GXLT_error_cpy_t
{
    string filename;
    string message;
    string cause;
    string type;
    uint32_t line;
};

class _GXA_
{
    _GXA_config_t config;
    vector<_GXA_label_t> labels;
    vector<_GXLT_error_cpy_t> runtime_errors;

    vector<logictype_t> vir_regs = {0, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<logictype_t> vir_stack;
    vector<string> vir_regs_names = {"x", "y", "t", "r0", "r1", "r2", "r3", "r4", "r5"};
    vector<string> special_vir_regs_name = {"t", "x", "y"};

    vector<_GXA_external_argument_t> external_arguments;
    vector<_GXA_external_t> externals;

    _GXA_label_t startLabel;
    string input;

    // uint64_t vir_sp = 0; // --- //

    vector<_GXA_snippet_t> _transpile_(string content, uint64_t line_idx);

    vector<_GXA_external_argument_t> _parse_argular_(string path, uint32_t index);
    void _open_extern_(string path, string name, uint32_t index);

public:
    vector<_GXA_snippet_t> output;
    uint8_t status = 0;

    void _pre_processors_(string &content, vector<uint32_t> ignore_lines);
    vector<_GXA_label_t> _slice_labels_(string content);

    _GXA_(string content, _GXA_config_t config);
    _GXA_();
    ~_GXA_();
    void init(string content, _GXA_config_t config);
    vector<_GXA_snippet_t> compile();
};
#endif
