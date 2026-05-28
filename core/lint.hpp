#ifndef LINT_HPP
#define LINT_HPP
#include "./types.hpp"
class _GXLT_
{
    vector<_GXA_external_t> externals;
    vector<_GXA_external_argument_t> external_arguments;
    vector<_GXA_label_t> labels;
    vector<string> vir_regs_names;
    vector<logictype_t> vir_regs;

public:
    vector<_GXLT_error_t> total_errors;
    vector<_GXLT_error_t> check_errors(string filename, string content, uint32_t index);
    vector<_GXLT_error_t> check_pre_processors_errors(string filename, string input);
    vector<_GXLT_error_t> check_label_errors(string filename);

    _GXLT_();
    _GXLT_(vector<_GXA_external_t> &external, vector<_GXA_external_argument_t> &external_arguments, vector<_GXA_label_t> &labels, vector<string> &vir_regs_name, vector<logictype_t> &vir_regs);
    void init(vector<_GXA_external_t> &external, vector<_GXA_external_argument_t> &external_arguments, vector<_GXA_label_t> &labels, vector<string> &vir_regs_name, vector<logictype_t> &vir_regs);
};

#endif
