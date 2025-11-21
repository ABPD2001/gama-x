#ifndef _GXA__HPP
#define _GXA__HPP
#include <string>
#include <cmath>
#include <iostream>
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

#define CMD parts[0]
using std::cout;
using std::max;
using std::min;
using std::string;
using std::to_string;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;

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

struct _GXA_snippet_t
{
	uint64_t t; // time
	uint32_t x; // x value
	uint32_t y; // y value
};

struct _GXA_config_t
{
	vector<string> reg_names = vector<string>();
	vector<double> reg_values = vector<double>();
};

class _GXA_
{
	_GXA_config_t config;
	vector<_GXA_snippet_t> output;
	vector<_GXA_label_t> labels;
	vector<double> vir_regs = {0, 255, 0, 0, 0, 0, 0, 0, 0, 0};
	vector<double> vir_stack;
	vector<string> vir_regs_names = {"x", "y", "t", "r0", "r1", "r2", "r3", "r4", "r5", "st"};
	string input;
	string startLabel;

	vector<_GXA_snippet_t> _transpile_(string content, uint16_t line_idx);
	vector<_GXA_label_t> _slice_labels_(string content);
	void _pre_processors_(string &content);

public:
	vector<_GXA_error_t> errors;
	uint8_t status = 0;

	_GXA_(string content, _GXA_config_t config);
	_GXA_();
	~_GXA_();
	vector<_GXA_snippet_t> compile();
};
#endif
