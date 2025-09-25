#ifndef UVA_ASSEMBLER_INSTRUCTION_UTILS
#define UVA_ASSEMBLER_INSTRUCTION_UTILS
#include <string>
#include <vector>
#include <cstdint>
#define CMD parts[0]

using std::string;
using std::vector;
using std::uint64_t;
using std::uint32_t;
using std::uint8_t;

struct _uva_error_t {
	string message;
	string cause;
	string type;
	uint32_t idx;
};

struct _uva_snippet_t {
	uint64_t t; // time
	uint32_t x; // x position
	uint32_t y; // y position
	bool v; // actual value
};

struct _uva_common_t {
	string name;
	uint8_t paramc;
};

struct _uva_instruction_t:_uva_common_t {
	(*bool error_check) (vector<string> params, vector<_uva_error_t> &errors);
	(*void run)(vector<_uva_snippet_t> &snippets,vector<string> params);
};

struct _uva_pre_processor_t:_uva_common_t {
	(void *run)(string &content);
	(void *check_error)(vector<_uva_error_t> &errors, vector<string> params);
};

class UVA_ASSEMBLER_INSTRUCTIONS {
	vector<_uva_instruction_t> instructions;
	vector<_uva_error_t> *errors;
	vector<_uva_snippet_t> output;

	public:
		void run(string line);
		UVA_ASSEMBLER_INSTRUCTIONS();
		UVA_ASSEMBLER_INSTRUCTIONS(vector<_uva_instruction_t> instructions, vector<_uva_error_t> *errors);
};

#endif
