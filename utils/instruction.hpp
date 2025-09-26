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
	string line;
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
	(bool *error_check) (vector<_uva_error_t> *errors,vector<string> params);
	(void *run)(vector<_uva_snippet_t> &snippets,vector<string> params);
};

struct _uva_pre_processor_t:_uva_common_t {
	(void *check_error)(vector<_uva_error_t> *errors, vector<string> params);
	(void *run)(string &content);
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
class UVA_ASSEMBLER_PRE_PROCESSORS {
	vector<_uva_pre_processor_t> pre_processors;
	vector<_uva_error_t> *errors;
	
	public:
		void run(string &content,string line);
		UVA_ASSEMBLER_PRE_PROCESSORS();
		UVA_ASSEMBLER_PRE_PROCESSORS(vector<_uva_pre_processor_t> pre_processors,vector<_uva_error_t> *errors);
};
#endif
