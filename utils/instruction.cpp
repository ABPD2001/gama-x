#include "./instruction.hpp"
#include "./string.hpp"

UVA_ASSEMBLER_INSTRUCTIONS::UVA_ASSEMBLER_INSTRUCTIONS(){};
UVA_ASSEMBLER_INSTRUCTIONS::UVA_ASSEMBLER_INSTRUCTIONS(vector<_uva_instruction_t> instructions, vector<_uva_error_t> *errors){
	this->instructions = instructions;
};

_uva_snippet_t UVA_ASSEMBLER_INSTURCTIONS::run(string line){
	const char invalid_chars[] = {'\r','\n','\t',' ','\f','\b','\a'};

	if(!filter(line,invalid_chars,7).length()) return;
	const vector<string> parts = split(line,' ');
	const vector<string> params (parts.begin()+1,parts.end());
	bool errored = false;

	for(_uva_instruction_t instruct:this->instructions){
		if(instruct.name == CMD) {
			if(params.size() != instruct.paramc) {
				this->errors->push_back((_uva_error_t) {"'"+CMD+"' instruction set excepts "+to_string(instruct.paramc)+" but recieved "+to_string(params.size())+" arguments!","ARGUMENTS_MISSED","value",line});
			errored = true;
			}
			for(string p:params){
				if(filter(p,const_cast<char[7]>(invalid_chars),7).empty()) {
					this->errors->push_back((_uva_error_t) {"'"+CMD+"' instruction set recived invalid syntax of argument!","ARGUMENT_INVALID_SYNTAX","syntax",,line})			
					errored = true;
				}
			}
			errored = errored || (*instruct.check_error)(const_cast<vector<string>>(&params), *this->errors);
			if(errored) return;
			(*instruct.run)(this->output,params);
			break;
		}
	}
}


UVA_ASSEMBLER_PRE_PROCESSORS::UVA_ASSEMBLER_PRE_PROCESSORSS(vector<_uva_pre_processor_t> pre_processors, vector<_uva_error_t> *errors){
	this->pre_processors = pre_processors;
	this->errors = errors;
};

UVA_ASSEMBLER_PRE_PROCESSORS::UVA_ASSEBLER_PRE_PROCESSORS(){}

void UVA_ASSEMBLER_PRE_PROCESSORS::run(string &content, string line){
	const vector<string> parts = split(line,' ');
	const vector<string> params (parts.begin()+1, parts.end());
	bool errored = false;

	for(_uva_pre_processor_t pre_processor:this->pre_processors){
		if(pre_processor.name == CMD){
			if(params.size() != pre_processor.paramc) {
				this->errors->push_back((_uva_error_t) {"'"+CMD+"' pre-processor set excepted "+to_string(pre_processor.paramc)+" arguments but recived "+to_string(params.size())+" arguments!","PRE_PROCESSOR_ARGUMENTS_MISSED","value",line});
				errored = true;
			}

			for(string p:params){
				const char invalid_chars[] = {'\r','\b','\t','\n','\a',' ','\v'};
				if(filter(p,const_cast<char[7]>(invalid_chars),7).empty()) {this->errors->push_back((_uva_error_t) {"'"+CMD+"' pre-processor set recived invalid syntax of value!","INVALID_ARGUEMENT_SYNTAX","syntax",line});
				errored = true;
	}
			}
			errored = errored || (*pre_processor.check_error)(this->errors, const_cast<vector<string>> params);
			if(errored) return;
			(*pre_processor.run)(content);
			break;
		}
	}
}
