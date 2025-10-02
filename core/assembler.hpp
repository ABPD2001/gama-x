#ifndef UVA_ASSEMBLER_HPP
#define UVA_ASSEMBLER_HPP
#include <string>
#include <cmath>
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

#define CMD parts[0]
using std::string;
using std::min;
using std::max;
using std::to_string;
using std::uint8_t;
using std::uint32_t;
using std::uint16_t;

struct _uva_label_t {
	string text;
	string name;
	uint32_t line_idx;
};

struct _uva_error_t {
	string message;
	string cause;
	string type;
	string line;
};

struct _uva_snippet_t {
	uint64_t t; // time
	uint32_t x; // x position
	uint32_t y; // y position
	bool v; // actual value
};

struct _uva_config_t {
	vector<string> reg_names = vector<string>();
	vector<double> reg_values = vector<double>();
	uint32_t y_max;
	uint32_t x_max;
};

class UVA_ASSEMBLER {
	_uva_config_t config;
	vector<_uva_snippet_t> output;
	vector<_uva_label_t> labels;
	vector<double> vir_regs = {0,255,0,0,0,0,0,0,0,0};
	vector<double> vir_stack;
	vector<string> vir_regs_names =  {"x","y","v","a","sleep","r0","r1","r2","r3","r4","r5","st"};
	string input;
	string startLabel;

	vector<_uva_snippet_t> _transpile_(string content, vector<string> vir_regs_names, vector<double> &vir_regs);
	vector<_uva_label_t> _slice_labels_(string content);
	void _pre_processors_(string &content);

	public:
		vector<_uva_error_t> errors;
		uint8_t status = 0;

		UVA_ASSEMBLER(string content, _uva_config_t config);
		UVA_ASSEMBLER();
		~UVA_ASSEMBLER();
		string compile();
};
#endif
