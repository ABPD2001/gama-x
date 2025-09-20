#include "./assembler.hpp"

UVA_ASSEMBLER::UVA_ASSEMBLER(vector<string> files_in, string file_out, bool verbose){
	//if(!exists(file_in)) {
	//	this->status = 1;
	//	return;
	//}
	//
	for(string f:files_in){
		if(!exists(f)) {
			this->status = 1;
			return;
		}
	}
	
	this->verbose = verbose;
	if(files_in.size()) this->file_o.open(file_out,ios::out);
	
	else {
		this->status = 1;
		return;
	}
	this->files_in = files_in;
	this->file_out = file_out;
	if(file_r.rdstate() != ios::goodbit) this->status = -1;
	else if(file_o.rdstate() != ios::goodbit) this->status = -2;

	this->status = 0;
}

string UVA_ASSEMBLER::_transpile_(string content,const vector<string> vir_regs_names, vector<double> &vir_regs,uint32_t idx){
	string output = "";
	string temp_line;

	signed long int cmp = 0;
	vector<string> lines = split(content,'\n');
	char empties[7] = {'\t','\n','\f','\r',' ','\a','\v'};
	
	for(uint32_t i = 0; i<lines.size(); i++){
		if(!filter(lines[i],empties,7).length()) continue;
		
		temp_line = trim(lines[i]);
		const vector<string> parts = split(temp_line,' ');
		const vector<string> params = split(join(vector<string>(parts.begin()+1,parts.end())," "),',');

			if(CMD == "mov"){
				signed short int tar_reg_idx = -1;
				if(params.size() != 2) {
					this->errors.push_back((_uva_error_t){"'mov' instrction set values missed!","MISSED_VALUE","syntax",i+idx});
					continue;
				}

				tar_reg_idx = find(vir_regs_names,params[0]);
				if(tar_reg_idx != -1 && params[1].length()) {
					if(params[1][0] == '#') vir_regs[tar_reg_idx] = to_double(trim(params[1]));
					else {
						signed short int reg2 = find(vir_regs_names,params[1]);
						vir_regs[tar_reg_idx] = vir_regs[reg2];
					}
				}
				else this->errors.push_back((_uva_error_t){"'mov' instruction set value missed or is invalid!","MISSED_OR_INVALID_REGISTER_NAME","syntax",i+idx});
			}
			else if(CMD == ".transpile") {
				if(vir_regs[3]) {
					vir_regs[1] = 255;
					vir_regs[0] = 0;
				}

				output+=join(vector<string>({to_string((int)vir_regs[0]),to_string((int)vir_regs[1]),to_string(vir_regs[2]>0),to_string((int) vir_regs[4])}),"-")+'\n';
			}
			else if(CMD == "rst"){
				vir_regs[0] = 0;
				vir_regs[1] = 255;
				vir_regs[2] = 0;
				vir_regs[3] = 0;
				vir_regs[4] = 0;
			}
			else if(CMD == "jmp"){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jmp' instruction set must have line index to jump!","MISSED_VALUE","syntax",i+idx});
				else if(isValidNumber(params[0])) i = min(lines.size()-1,to_uint32(params[0]));
			}
			else if(CMD == "cmp"){
				if(params.size()!=2) this->errors.push_back((_uva_error_t){"'cmp' instruction set values missed!","MISSED_VALUE","syntax",i+idx});
				const signed short int reg1_idx = find(this->vir_regs_names,params[0]);
				const signed short int reg2_idx = find(this->vir_regs_names,params[1]);

				if(reg1_idx > -1 && reg2_idx > -1) cmp = vir_regs[reg1_idx]-vir_regs[reg2_idx];
				else this->errors.push_back((_uva_error_t){"'cmp' instruction set values are invalid!","INVALID_VALUE","value",i+idx});
			}
			else if(CMD == "jeq" && !cmp) {
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jeq' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
				else if(params[0].length()) i = min(lines.size()-1,to_uint32(params[0]));
				else this->errors.push_back((_uva_error_t){"'jeq' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			} 
			else if(CMD == "jgr" && cmp){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jgr' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length()) i = min(lines.size()-1,to_uint32(params[0]));
                                else this->errors.push_back((_uva_error_t){"'jgr' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}
			else if(CMD == "jlw" && cmp < 0){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jlw' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length()) i = min(lines.size()-1,to_uint32(params[0]));
                                else this->errors.push_back((_uva_error_t){"'jlw' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}
			else if(CMD == "jeg" && cmp >= 0){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jeg' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length()) i = min(lines.size()-1,to_uint32(params[0]));
                                else this->errors.push_back((_uva_error_t){"'jeg' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}
			else if(CMD == "jle" && cmp <= 0){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'jle' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length()) i = min(lines.size()-1,to_uint32(params[0]));
                                else this->errors.push_back((_uva_error_t){"'jle' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}
			else if(CMD == "add"){
				if(params.size()!=2) this->errors.push_back((_uva_error_t){"'add' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length() && params[1].length()){
					const signed short int reg_idx = find(vir_regs_names,params[0]);
					signed long int reg2 = 0;

					if(params[1][0] == '#') reg2 = to_double(params[1].substr(1,params[1].length()));
					else reg2 = vir_regs[find(vir_regs_names,params[1])];

					if(reg_idx > -1) vir_regs[reg_idx] += reg2;
					else this->errors.push_back((_uva_error_t){"'add' instruction set value is invalid!","INVALID_VALUES","value",i+idx});
				}
                                else this->errors.push_back((_uva_error_t){"'add' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}

			else if(CMD == "incr"){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'incr' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
				else if(params[0].length()){
					const signed short int reg_idx = find(vir_regs_names,params[0]);
					if(reg_idx > -1) vir_regs[reg_idx]++;
					else this->errors.push_back((_uva_error_t){"'incr' instructions set value is invalid!","INVALID_VALUE","value",i+idx}); 
				}	
			}
			
			else if(CMD == "sub"){
				if(params.size()!=2) this->errors.push_back((_uva_error_t){"'min' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
                                else if(params[0].length() && params[1].length()){
                                        const signed short int reg_idx = find(vir_regs_names,params[0]);
                                        signed long int reg2 = 0;

                                        if(params[1][0] == '#') reg2 = to_double(params[1].substr(1,params[1].length()));
                                        else reg2 = vir_regs[find(vir_regs_names,params[1])];

                                        if(reg_idx > -1) vir_regs[reg_idx] -= reg2;
                                        else this->errors.push_back((_uva_error_t){"'add' instruction set value is invalid!","INVALID_VALUES","value",i+idx});
                                }
                                else this->errors.push_back((_uva_error_t){"'add' instruction set value is invalid!","INVALID_VALUE","value",i+idx});
			}

			else if(CMD == "decr"){
				if(params.size()!=1) this->errors.push_back((_uva_error_t){"'decr' instructions set values missed!","MISSED_VALUE","value",i+idx});
                                else if(params[0].length()){
                                        const signed short int reg_idx = find(vir_regs_names,params[0]);
                                        if(reg_idx > -1) vir_regs[reg_idx]--;
                                        else this->errors.push_back((_uva_error_t){"'decr' instructions set value is invalid!","INVALID_VALUE","value",i+idx});
                                }
			}
			else if(CMD == "push"){
				if(params.size() != 1 || params[0].size()) this->errors.push_back((_uva_error_t) {"'push' instruction set values missed!","MISSED_VALUE","value"});
				else {
					if(params[0][0] == '#') this->vir_stack.push_back(to_double(trim(params[0])));
					else {
						const short int reg = find(this->vir_regs_names,params[0]);
						if(reg > -1) this->vir_stack.push_back(this->vir_regs[reg]);
						else this->errors.push_back((_uva_error_t){"'push' instruction set value is invalid!","INVALID_VLAUE","value"});
					}
				}
			}
			else if(CMD == "pull"){
				if(params.size() != 1 || params[0].empty()) this->errors.push_back((_uva_error_t){"'pull' instruction set value missed!","MISSED_VALUE","value"});
				else {
					const short int reg = find(this->vir_regs_names,params[0]);
					if(reg > -1) {
						this->vir_regs[reg] = this->vir_stack[this->vir_stack.size()-1];
						this->vir_stack = vector<double>(this->vir_stack.begin(),this->vir_stack.end()-1);
						
					}
					else this->errors.push_back((_uva_error_t){"'pull' instruction set value is invalid!","INVALID_VALUE","value"});
				}
			}
			else if(CMD == "call") {
			        if(!(params.size()>1 && params.size() < 3)) this->errors.push_back((_uva_error_t){"'call' instructions set values missed!","MISSED_VALUE","syntax",i+idx});
				else if(params[0].length()){
					string text;
					for (uint16_t i = 0; i<this->labels.size(); i++) {
						if(this->labels[i].name == params[0]) text = this->labels[i].text;
					}
					if(text.length()) {
						if(params.size() == 2) {
							if (params[1] == "EQ" && cmp) continue;
							else if(params[1] == "LE" && (cmp > 0 || !cmp)) continue;
						        else if(params[1] == "GE" && (cmp < 0 || !cmp)) continue;
							else if(params[1] == "NE" && !cmp) continue;	
						}
						output+=this->_transpile_(text,this->vir_regs_names,this->vir_regs, i+idx);
					}
					else this->errors.push_back((_uva_error_t){"'call' instructions set value is invalid!","INVALID_VALUE","value",i+idx}); 
				}
			}
			else if(CMD == "nop"){
				for(string p:params){
					for(int i = 0;i <to_double(p); i++){}
				}
			}
			else this->errors.push_back((_uva_error_t){"Invalid instruction entered!","INVALID_INSTRUCTION","decode",i+idx});
		}

	return output;
}

string UVA_ASSEMBLER::_co_processors_(string &content){
	string output = "";
	vector<string> lines = split(content,'\n');
	char empties[7] = {'\r','\n',' ','\a','\b','\t','\f'};

	for(uint32_t i = 0; i<lines.size(); i++){
		if(!filter(lines[i],empties,7).length()) continue;
		const vector<string> parts = split(lines[i],' ');

		if(CMD == ".replace") {
			if(parts.size() != 3 || parts[1].empty() || parts[2].empty()) this->errors.push_back((_uva_error_t){"'.replace' co-processor set values missed!","CO_PROCCESSOR_VALUES_MISSED","syntax",i});			
			else {
				content = replaceAll(content,parts[1],parts[2]);
				lines = split(content,'\n');
				lines[i] = "_NONE_";
			}
		}
		else if(CMD == ".info") {
			if(parts.size() != 5) this->errors.push_back((_uva_error_t){"'.info' co-proccessor set values missed!","CO_PROCCESSOR_VALUES_MISSED","syntax",i});

			else{
				bool isValid = true;
				const vector<string> params (parts.begin()+1,parts.end());

				for(string s:params){
					if(s.empty() || s.find(",") > -1){
						isValid = false;
						break;
					}
				}
				if(!isValid)  this->errors.push_back((_uva_error_t){"'.info' co-proccessor set values must be valid characters!","CO_PROCCESSOR_INVALID_VALUES","value",i});
				else {
					output ='{'+join(params,",")+"}\n"+output;
					lines = split(content,'\n');
					lines[i] = "_NONE_";

				}
			}			
		}
		else if(CMD == ".end") {
			lines[i] = "_NONE_";
			string out = "";
			for(string s:lines){
				if(s == "_NONE_") continue;
				out+=s+'\n';
			}
			content = out+'\n';
			break;

		}
		
		else if(CMD == ".main") {
			if(parts.size() != 2 || parts[1].empty()) this->errors.push_back((_uva_error_t){"'.main' co-proccessor set value missed!","CO_PROCCESSOR_VALUE_MISSED","syntax",i});
			else {
				this->startLabel = parts[1];
				lines[i] = "_NONE_";
			}
		}
	}

	return output+'\n';
}

vector<label_t> UVA_ASSEMBLER::_slice_labels_(string content){
	vector<label_t> output;
	vector<string> lines = split(content,'\n');
	string cur_label = "";
	uint32_t idx_line = -1;
	char empties[7] = {'\r','\n',' ','\t','\a'};

	if(this->verbose) cout<<"slicing labels...\n";

	for(uint32_t i = 0; i<lines.size(); i++){
		if(!filter(lines[i],empties,7).length()) continue;
		lines[i] = trim(lines[i]);

		if(cur_label.empty() && lines[i].find(':') != string::npos && lines.size()-1 != i) {
			cur_label = filter(filter(lines[i],':'),empties,7);
			for(label_t lbl:this->labels){
				if(lbl.name == cur_label) this->errors.push_back((_uva_error_t){"label "+cur_label+" overloaded!","LABEL_OVERLOADED","syntax"});
			}
			idx_line = i;
		}

		else if (lines[i] == "end" && cur_label.length() && idx_line != -1) {
			const string content = join(vector<string>(lines.begin()+idx_line+1,lines.begin()+i),"\n");
			label_t label = {content,cur_label,i};
			cur_label.clear();
			output.push_back(label);
		}
	}
	return output;
}

UVA_ASSEMBLER::~UVA_ASSEMBLER(){
	this->file_r.close();
	this->file_o.close();
	this->status = 3;
	this->errors.clear();
	this->labels.clear();
	this->files_in.clear();
	this->file_out.clear();
	this->startLabel.clear();
	this->vir_regs_names.clear();
	this->vir_regs.clear();
}

uint8_t UVA_ASSEMBLER::compile(){
	string file_content = "";
	string out;
	string temp_line;
	string file_content_result = "";
	
	for(string f:this->files_in){
	this->file_r.open(f,ios::in);
	if(this->verbose) cout<<"reading file "<<f<<"...";
	while(getline(this->file_r,temp_line)){
		file_content+=temp_line+'\n';
	}

	temp_line[temp_line.length()-1] = '\0';
	if(this->verbose) cout<<"readed.\nco-processing...\n";
	out = this->_co_processors_(file_content);
	file_content_result+=file_content;
	file_content_result+='\n';
	}
	file_content_result[file_content_result.length()-1] = '\0';

	this->labels = this->_slice_labels_(file_content_result);
	bool found = false;

	for(label_t lbl: this->labels){
		if(lbl.name == this->startLabel) {
			out += this->_transpile_(lbl.text,this->vir_regs_names,this->vir_regs,0);
			found = true;
			break;
		}
	}

	if(!found) {
		this->status=2;
		return this->status;
	}
	this->file_o << out;

	return this->status;
}
