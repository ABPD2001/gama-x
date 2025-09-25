#include "./instruction.hpp"
#include "./string.hpp"

UVA_ASSEMBLER_INSTRUCTIONS::UVA_ASSEMBLER_INSTRUCTIONS(){};
UVA_ASSEMBLER_INSTRUCTIONS::UVA_ASSEMBLER_INSTRUCTIONS(vector<_uva_instruction_t> instructions, vector<_uva_error_t> *errors){
	this->instructions = instructions;
};

_uva_snippet_t UVA_ASSEMBLER_INSTURCTIONS::run(string line){
	char invalid_chars[] = {'\r','\n','\t',' ','\f','\b','\a'};

	if(!filter(line,invalid_chars,7).length()) return;
	const vector<string> parts = split(line,' ');
	const vector<string> params (parts.begin()+1,parts.end());

	for(_uva_instruction_t instruct:this->instructions){
		if(instruct.name == CMD) {
			const string param_err_prefix = ;
			if(params.size() != instruct.paramc) {
				this->errors->push_back((_uva_error_t) {"'"+CMD+"' instruction set excepts "+to_string(instruct.paramc)+" but recieved "+to_string(params.size())+" arguments!","ARGUMENTS_MISSED"});
			return;
			}
			const bool errored = (*instruct.check_error)(const_cast<vector<string>>(&params), *this->errors);
			if(errored) return;
			(*instruct.run)(this->output,params);
			break;
		}
	}
}
