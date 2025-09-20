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

struct label_t {
	string text;
	string name;
	uint32_t line_idx;
};

class UVA_ASSEMBLER {
	fstream file_r;
	fstream file_o;
	vector<double> vir_regs = {0,255,0,0,0,0,0,0,0,0};
	vector<string> vir_regs_names =  {"x","y","v","a","sleep","r0","r1","r2","r3","r4","r5","st"};
	vector<label_t> labels;
	vector<double> vir_stack;

	vector<string> files_in;
	string file_out;
	string startLabel;
	uint32_t stack_limit;
	bool stack_disbled;
	bool verbose;


	string _transpile_(string content, const vector<string> vir_reg_names, vector<double> &vir_regs, uint32_t idx);
	vector<label_t> _slice_labels_(string content);
	string _co_processors_(string &content);
	public:
		vector<_uva_error_t> errors;
		uint8_t status = 0;
		UVA_ASSEMBLER(vector<string> files_in, string file_out, bool verbose);
		~UVA_ASSEMBLER();
		uint8_t compile();	
};
#endif
