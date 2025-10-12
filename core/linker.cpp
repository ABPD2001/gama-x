#include "./linker.hpp"

_GAMA_X_LINKER_::_GAMA_X_LINKER_(vector<string> contents) {
	this->contents = contents;
}

_GAMA_X_LINKER::_GAMA_X_LINKER_(){}

void _GAMA_X_LINKER_::_slice_labels(string content){
	vector<_GAMA_X_LINKER_label_t> output;
	vector<string> lines = split(content, '\n');
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
			for (_uva_label_t lbl : this->labels)
			{
				if (lbl.name == cur_label)
				{
					this->errors.push_back((_uva_error_t){"label " + cur_label + " overloaded!", "LABEL_OVERLOADED", "syntax", i});
					break;
				}
			}
			idx_line = i;
		}

		else if (lines[i].substr(0,3) == "end" && cur_label.length() && idx_line != -1)
		{
			const string content = join(vector<string>(lines.begin() + idx_line + 1, lines.begin() + i), "\n");
			_uva_label_t label = {content, cur_label, (uint32_t) idx_line};
			cur_label.clear();
			output.push_back(label);
		}
	}
	this->labels.push_back(output);	
}

string _GAMA_X_LINKER_::_merge_pre_processors_(vector<_GAMA_X_LINKER_file_t> files) {
	vector<string> pre_processors_sections;
	for(uint32_t i = 0; i<filed.size(); i++){
		string pre_processors = files[i].content.substr(0,contents[i].find(".end"));
		
		bool isModule = pre_processors.find(".module") != string::npos;
		bool hasMainPoint = pre_processors.find(".main") != string::npos;

		if(isModule && hasMainPoint){
			this->errors.push_back((_GAMA_X_LINKER_error_t) {"invalid file type","file-type","file "+pre_processors[i].name+" cannot be main type and module type at the same time!",-1} );
		continue;
		}
		pre_processors_sections.push_back(pre_processors);
	}
	return join(pre_processors_sections,"\n");
}

string _GAMA_X_LINKER_::_merge_instructions_(vector<_GAMA_X_LINKER_file_t> files){
	vector<string> instructions;
	for(uint32_t i = 0; i<files.size(); i++){
		const int idx = files[i].content.name(".end");
		if(idx == string::npos){
			this->errors.push_back((_GAMA_X_LINKER_error_t){"invalid file pre-processors","file-content","file "+files[i].name+" pre-processors input has not any pre-processors end input instruction!",-1});
			continue;
		}
		string file_instructions = files[i].content.substr(idx+3,files[i].content.length()-(idx+3));
		const uint16_t errors_len = this->errors.size();
		this->check_errors_(file_instructions);
		if(errors_len != this->errors.size()) continue;
		instructions.push_back(file_instructions);
	}

	return join(instructions,"\n");
}

bool _mov_ec_(string line){
	
}
bool _jmp_ec_(string line){

}
bool _cmp_ec_(string line){

}
bool _add_ec_(string line){

}
bool _incr_ec_(string line){

}
bool _sub_ec_(string line){

}
bool _decr_ec_(string line){

}
bool _push_ec_(string line){

}
bool _pull_ec_(string line){

}
bool _call_ec_(string line){

}
bool _nop_ec_(string line){

}

void _GAMA_X_LINKER_::_check_errors_(_GAMA_X_LINKER_file_t file){
	const _GAMA_X_LINKER_err_check_t error_checks[] = {_mov_ec_,_jmp_ec_,_cmp_ec_,_add_ec_,_incr_ec_,_sub_ec_,_decr_ec_,_push_ec_,_pull_ec_,_call_ec_,_nop_ec_};
	const vector<string> instruction_names = {"mov","cmp","add","incr","sub","decr","push","pull","call","nop"};
	const vector<string> lines = split(file.content,'\n');

	for(uint32_t i = 0; i<lines.size(); i++){
		const vector<string> parts = split(lines[i],' ');
		const vector<string> params = parts.size() > 1 ? split(parts[1],',') : {};
		
		if(parts.size()>2) {
			this->errors.push_back((_GAMA_X_LINKER_error_t){"invalid instruction syntax","syntax","instruction syntax is invalid, extra space detected!"});
			continue;
		}
		
		const short int instruction_idx = find(instruction_names,parts[0]);	
		if(error_checks[instruction_idx](lines[i])) continue;
	}
}
