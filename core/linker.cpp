#include "./linker.hpp"

_GAMA_X_LINKER_::_GAMA_X_LINKER_(vector<_GAMA_X_LINKER_file_t> files) {
	this->files = files;
}

_GAMA_X_LINKER_::_GAMA_X_LINKER_(){}

void _GAMA_X_LINKER_::_slice_labels_(_GAMA_X_LINKER_file_t file){
	vector<_GAMA_X_LINKER_label_t> output;
	vector<string> lines = split(file.content, '\n');
	string cur_label = "";

	int idx_line = -1;
	char empties[7] = {'\r', '\n', ' ', '\t', '\a'};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		if (!filter(lines[i], empties, 7).length())
			continue;
		lines[i] = trim(lines[i]);

		if(cur_label.empty() && lines[i].find(':') != string::npos && lines.size() - 1 != i)
		{
			cur_label = filter(filter(lines[i], ':'), empties, 7);
			for (uint32_t i = 0; i<output.size(); i++)
			{
				const _GAMA_X_LINKER_label_t lbl = output[i];

				if (lbl.name == cur_label)
				{
					this->errors.push_back((_GAMA_X_LINKER_error_t){"label overloaded! ("+file.name+")", "syntax", "Label "+cur_label+" overloaded!",i});
					break;
				}
			}
			idx_line = i;
		}
		else if (lines[i].substr(0,3) == "end" && cur_label.length() && idx_line != -1)
		{
			const string content = join(vector<string>(lines.begin() + idx_line + 1, lines.begin() + i), "\n");
			_GAMA_X_LINKER_label_t label = {content, cur_label, (uint32_t) idx_line};
			cur_label.clear();
			output.push_back(label);
		}
	}
	this->labels.push_back(output);	
}

string _GAMA_X_LINKER_::_merge_pre_processors_(vector<_GAMA_X_LINKER_file_t> files) {
	vector<string> pre_processors_sections;

	for(uint32_t i = 0; i<files.size(); i++){
		string pre_processors = files[i].content.substr(0,files[i].content.find(".end"));
		
		bool isModule = pre_processors.find(".module") != string::npos;
		bool hasMainPoint = pre_processors.find(".main") != string::npos;

		if(isModule && hasMainPoint){
			this->errors.push_back((_GAMA_X_LINKER_error_t) {"invalid file type","file-type","file "+files[i].name+" cannot be main type and module type at the same time!",-1} );
		continue;
		}
		pre_processors_sections.push_back(pre_processors);
	}
	return join(pre_processors_sections,"\n");
}

string _GAMA_X_LINKER_::_merge_instructions_(vector<_GAMA_X_LINKER_file_t> files){
	vector<string> instructions;
	for(uint32_t i = 0; i<files.size(); i++){
		const int idx = files[i].content.find(".end");
		
		if(idx == string::npos){
			this->errors.push_back((_GAMA_X_LINKER_error_t){"invalid file pre-processors","file-content","file "+files[i].name+" pre-processors input has not any pre-processors end input instruction!",-1});
			continue;
		}

		string file_instructions = files[i].content.substr(idx+3,files[i].content.length()-(idx+3));
		const uint16_t errors_len = this->errors.size();
		
		this->_check_errors_(file_instructions,files[i].name);
		if(errors_len != this->errors.size()) continue;
		
		instructions.push_back(file_instructions);
	}

	return join(instructions,"\n");
}

bool _mov_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params,_GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 2) {
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'mov' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _jmp_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params,_GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 1){
		errors.push_back((_GAMA_X_LINKER_error_t) {"Invalid arguments syntax ("+file.name+")","syntax","'jmp' instruction set arguments are out of range!",line});
		return true;
	}
	if(!isValidNumber(params[0])){
		errors.push_back((_GAMA_X_LINKER_error_t) {"Invalid argument type ("+file.name+")","value","'jmp' instruction set argument value is invalid (must be a valid number)!",line});
		return true;
	}
	return false;
}
bool _cmp_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 2){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'cmp' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _add_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 2){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'add' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _incr_ec_(vector<_GAMA_X_LINKER_error_t> &errors, vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 1){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'incr' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _sub_ec_(vector<_GAMA_X_LINKER_error_t> &errors, vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 2){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'sub' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _decr_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 1){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'decr' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _push_ec_(vector<_GAMA_X_LINKER_error_t> &errors, vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 1){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'push' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _pull_ec_(vector<_GAMA_X_LINKER_error_t> &errors, vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() != 1){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'pull' instruction set arguments are out of range!",line});
		return true;
	}
	return false;
}
bool _call_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size() < 1 || params.size() > 2){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'call' instruction set arguments are out of range!",line});
		return true;
	}
	else if(params.size() == 2){
		string conditions[] = {"NE","AL","EQ","GE"};
		bool isCorrect = false;
		for(uint8_t i = 0; i<4; i++){
			if (conditions[i] == params[1]) {
				isCorrect = true;
				break;
			}
		}
		if(!isCorrect){
			errors.push_back((_GAMA_X_LINKER_error_t) {"Invalid arguments syntax ("+file.name+")","syntax","'call' instruction set arguments are out of range!",line});
			return true;
		}
	}
	return false;
}
bool _nop_ec_(vector<_GAMA_X_LINKER_error_t> &errors,vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	const uint16_t errors_c = errors.size();
	for(string p:params){
		if(!isValidNumber(p)) 
			errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'nop' instruction set arguments are out of range!",line});
	}
	return !(errors_c != errors.size());
}
bool _transpile_ec_(vector<_GAMA_X_LINKER_error_t> &errors, vector<string> params, _GAMA_X_LINKER_file_t file, uint32_t line){
	if(params.size()){
		errors.push_back((_GAMA_X_LINKER_error_t){"Invalid arguments syntax ("+file.name+")","syntax","'.transpile' instruction set arguments are out of range!",line});
		return true;
	}
	return false;	
}

void _GAMA_X_LINKER_::_check_errors_(string content, string filename){
	_GAMA_X_LINKER_err_check_t *error_checks[] = {_mov_ec_,_jmp_ec_,_cmp_ec_,_add_ec_,_incr_ec_,_sub_ec_,_decr_ec_,_push_ec_,_pull_ec_,_call_ec_,_nop_ec_,_transpile_ec_};

	const vector<string> instruction_names = {"mov","cmp","add","incr","sub","decr","push","pull","call","nop",".transpile"};
	const vector<string> lines = split(content,'\n');
	char empties[4] = {' ','\t','\n','\r'};

	for(uint32_t i = 0; i<lines.size(); i++){
		if(lines[i].find(":") != string::npos || filter(lines[i],empties,4).empty()) continue;

		const vector<string> parts = split(lines[i],' ');
		const vector<string> params = (parts.size() > 1 ? split(parts[1],',') : vector<string>());
		
		if(parts.size()>2) {
			this->errors.push_back((_GAMA_X_LINKER_error_t){"invalid instruction syntax ("+filename+")","syntax","instruction syntax is invalid, extra space detected!",i});
			continue;
		}
		
		const short int instruction_idx = find(instruction_names,parts[0]);	
		if(!(*error_checks[instruction_idx])(this->errors,params,(_GAMA_X_LINKER_file_t) {"",filename},i)) continue;
	}
}

string _GAMA_X_LINKER_::merge(){
	const string pre_processors = this->_merge_pre_processors_(this->files);
	const string instructions = this->_merge_instructions_(this->files);
	
	for(_GAMA_X_LINKER_file_t f:this->files){
		this->_slice_labels_(f);
		this->_check_errors_(f.content,f.name);
	}

	if(this->errors.size()) return "";
	
	return (pre_processors+'\n'+instructions);
}
