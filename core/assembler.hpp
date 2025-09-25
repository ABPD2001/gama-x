#ifndef MARKUP_HPP
#define MARKUP_HPP
#include <string>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <iostream>
#include <stack>
#include "../utils/string.hpp"
#include "../utils/vector.hpp"
#include "../utils/number.hpp"

#define CMD parts[0]
using std::string;
using std::fstream;
using std::ios;
using std::min;
using std::max;
using std::to_string;
using std::getline;
using std::uint8_t;
using std::uint32_t;
using std::uint16_t;
using std::cout;

namespace fs = std::filesystem;
using fs::exists;

struct _uva_error_t {
	string message;
	string cause;
	string type;
	uint32_t idx;
};

struct _uva_label_t {
	string text;
	string name;
	uint32_t line_idx;
};

struct _uva_snippet_t {
	uint64_t time;
	uint32_t x;
	uint32_t y;
	bool value;
};

struct _uva_config_t {
	vector<string> names = {};
	vector<double> values = {};
	uint32_t x_max;
	uint32_t y_max;
	bool binary;
	bool verbose;
};

class UVA_ASSEMBLER {
	fstream file_r;
	fstream file_o;
	_uva_config_t config;
	vector<double> vir_regs = {0,0,0,0,0,0,0,0,0,0};
	vector<string> vir_regs_names =  {"x","y","v","a","sleep","r0","r1","r2","r3","r4","r5","st"};
	vector<_uva_label_t> labels;
	vector<double> vir_stack;

	vector<string> files_in;
	string file_out;
	string startLabel;


	vector<_uva_snippet_t> _transpile_(string content, const vector<string> vir_reg_names, vector<double> &vir_regs, uint32_t idx);
	vector<_uva_label_t> _slice_labels_(string content);
	void _co_processors_(string &content);
	public:
		vector<_uva_error_t> errors;
		uint8_t status = 0;

		UVA_ASSEMBLER(vector<string> files_in, string file_out);
		~UVA_ASSEMBLER();
		uint8_t compile();
		void set_config(_uva_config_t config);	
};
#endif
