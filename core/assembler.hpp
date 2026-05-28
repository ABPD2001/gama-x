#ifndef _GXA__HPP
#define _GXA__HPP
#include <cmath>
#include "./types.hpp"

using std::ios;
using std::max;
using std::min;

using std::abs;
using std::ceil;
using std::cos;
using std::floor;
using std::log;
using std::pow;
using std::round;
using std::sin;
using std::sqrt;
using std::tan;

class _GXA_
{
    _GXA_config_t config;
    vector<_GXA_label_t> labels;
    vector<_GXLT_error_t> runtime_errors;

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
