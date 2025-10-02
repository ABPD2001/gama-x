#include "./assembler.hpp"

UVA_ASSEMBLER::UVA_ASSEMBLER(string content, _uva_config_t config = {}){
	this->input = content;
	this->config = config;
}
UVA_ASSEMBELR::UVA_ASSEMBLER(){}

string UVA_ASSEMBLER::_transpile_(string content,const vector<string> vir_regs_names, vector<double> &vir_regs){
	vector<string> lines = split(content,'\n');
	signed long int cmp = 0;
	const char empties[7] = {'\t','\n','\f','\r',' ','\a','\v'};
	

	for(uint32_t i = 0; i<lines.size(); i++){
		if(!filter(lines[i],empties,7).length()) continue;
		
		temp_line = trim(lines[i]);
		const vector<string> parts = split(temp_line,' ');
		const vector<string> params = split(join(vector<string>(parts.begin()+1,parts.end())," "),',');
		for(uint8_t i = 0; i<params.size(); i++){
			if(filter(params[i],const_cast<char[7]>(empties),7).empty()) this->errors.push_back((_uva_error_t) ({"'"+CMD+"' instruction set arguemnt "+to_string(i+1) + " has invalid syntax!","INVALID_VALUE_SYNTAX","syntax",lines[i]}));
		};


			if(CMD == "mov"){
				signed short int tar_reg_idx = -1;
				if(params.size() != 2) {
					this->errors.push_back((_uva_error_t){"'mov' instrction set arguments missed!","MISSED_ARGUMENS","value",line});
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
					vir_regs[1] = 0;
					vir_regs[0] = this->config.x_max;
				}
				_uva_snippet_t snippet = {(uint64_t) vir_regs[4], (uint32_t) (vir_regs[0]), (uint32_t) (vir_regs[1]), (bool) (vir_regs[2] > 0)};
				output.push_back(snippet);
			}
			else if(CMD == "rst"){
				vir_regs[0] = 0;
				vir_regs[1] = this->config.y_max;
				vir_regs[2] = 0;
				vir_regs[3] = 0;
				vir_regs[4] = 0;
			}
			else if(CMD == "hrst"){
				vir_regs[1] = this->config.x_max;
				for(uint16_t i = 0; i<this->vir_regs.size(); i++){
					this->vir_regs[i] = 0;
				}
			}
			else if(CMD == "jmp"){
				if(params.size()>2 || params.size()<1) this->errors.push_back((_uva_error_t){"'jmp' instruction set must have line index to jump!","MISSED_VALUE","syntax",i+idx});
				else if(isValidNumber(params[0])) {
					const uint32_t num = to_uint32_t(params[0]);
					if(num > lines.size()-1 || num < 0) {
						this->errors.push_back((_uva_error_t) {"'jmp' instruction set argument is over/under of current block!","INVALID_JMP_PLACE","value",lines[i]});
						continue;
					}
					if(params.size() == 2) {
						if (params[1] == "EQ" && cmp) continue;
						else if(params[1] == "LE" && cmp >= 0) continue;
						else if(params[1] == "GE" && cmp <= 0) continue;
						else if(params[1] != "AL") {
							this->errors.push_back((_uva_error_t) {"'jmp' instruction set condition is invalid!","INVALID_CONDITION","value",line});
						continue;
						}
					}
					i = min(lines.size()-1,num);
				}
				else this->errors.push_back((_uva_error_t){"'jmp' instruction set jump address has invalid syntax!","INVALID_ARGUEMNT_SYNTAX","syntax",line});
			}
			else if(CMD == "cmp"){
				if(params.size()!=2) {
					this->errors.push_back((_uva_error_t){"'cmp' instruction set arguments are missed!","MISSED_ARGUMENTS","value",lines[i]});
					continue;
				}
				const signed short int reg1_idx = find(this->vir_regs_names,params[0]);
				const signed short int reg2_idx = find(this->vir_regs_names,params[1]);

				if(reg1_idx > -1 && reg2_idx > -1) cmp = vir_regs[reg1_idx]-vir_regs[reg2_idx];
				else this->errors.push_back((_uva_error_t){"'cmp' instruction set registers are invalid!","INVALID_COMPARE_REGISTER","value",lines[i]});
			}
			else if(CMD == "add"){
				if(params.size()!=2) {
					this->errors.push_back((_uva_error_t){"'add' instructions set arguments missed!","MISSED_ARGUMENTS","value",lines[i]});
					continue;
				}
					const signed short int reg_idx = find(vir_regs_names,params[0]);
					signed long int reg2 = 0;

					if(params[1][0] == '#') reg2 = to_double(params[1].substr(1,params[1].length()));
					else reg2 = vir_regs[find(vir_regs_names,params[1])];

					if(reg_idx > -1) vir_regs[reg_idx] += reg2;
					else this->errors.push_back((_uva_error_t){"'add' instruction set arguements are invalid!","INVALID_ARGUMENTS","value",lines[i]});
			}

			else if(CMD == "incr"){
				if(params.size()!=1) {
					this->errors.push_back((_uva_error_t){"'incr' instructions set arguments are missed!","MISSED_ARGUMENTS","value",lines[i]});
					continue;
				}
					const signed short int reg_idx = find(vir_regs_names,params[0]);
					if(reg_idx > -1) vir_regs[reg_idx]++;
					else this->errors.push_back((_uva_error_t){"'incr' instructions set argmuents are invalid!","INVALID_ARGUMENTS","value",lines[i]});
			}
			
			else if(CMD == "sub"){
				if(params.size()!=2) {
					this->errors.push_back((_uva_error_t){"'min' instructions set arguments missed!","MISSED_ARGUMENTS","value",lines[i]});
                                        continue;
				}

					const signed short int reg_idx = find(vir_regs_names,params[0]);
                                        signed long int reg2 = 0;

                                        if(params[1][0] == '#') reg2 = to_double(params[1].substr(1,params[1].length()));
                                        else reg2 = vir_regs[find(vir_regs_names,params[1])];

                                        if(reg_idx > -1) vir_regs[reg_idx] -= reg2;
                                        else this->errors.push_back((_uva_error_t){"'add' instruction set arguments are invalid!","INVALID_ARGUMENTS","value",lines[i]});
                                }

			else if(CMD == "decr"){
				if(params.size()!=1) {
					this->errors.push_back((_uva_error_t){"'decr' instructions set arguments missed!","MISSED_ARGUMENTS","value",lines[i]});
                                	continue;
				}
					else if(params[0].length()){
                                 	       const signed short int reg_idx = find(vir_regs_names,params[0]);
                                        	if(reg_idx > -1) vir_regs[reg_idx]--;
                                        	else this->errors.push_back((_uva_error_t){"'decr' instructions set arguments is invalid!","INVALID_ARGUMENT","value",lines[i]});
                                	}
			}
			else if(CMD == "push"){
				if(params.size() != 1) {
					this->errors.push_back((_uva_error_t) {"'push' instruction set arguments missed!","MISSED_ARGUMENTS","value"});
					continue;
				}
					if(params[0][0] == '#') this->vir_stack.push_back(to_double(trim(params[0])));
					else {
						const short int reg = find(this->vir_regs_names,params[0]);
						if(reg > -1) this->vir_stack.push_back(this->vir_regs[reg]);
						else this->errors.push_back((_uva_error_t){"'push' instruction set register is invalid!","INVALID_STACK_REGISTER","value",lines[i]});
					}
				
			}
			else if(CMD == "pull"){
				if(params.size() != 1) {
					this->errors.push_back((_uva_error_t){"'pull' instruction set arguments missed!","MISSED_ARGUMENTS","value",lines[i]});
					continue;
				}
					const short int reg = find(this->vir_regs_names,params[0]);
					if(reg > -1) {
						this->vir_regs[reg] = this->vir_stack[this->vir_stack.size()-1];
						this->vir_stack = vector<double>(this->vir_stack.begin(),this->vir_stack.end()-1);
						
					}
					else this->errors.push_back((_uva_error_t){"'pull' instruction set argument is invalid!","INVALID_ARGUMENT","value",lines[i]});
				
			}
			else if(CMD == "call") {
			        if(params.size()<1 && params.size() > 3) {
					this->errors.push_back((_uva_error_t){"'call' instructions set arguments missed!","MISSED_ARGUMENTS","value",lines[i]});
					continue;
				}
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
						else if(params[1] != "AL") {
							this->errors.push_back((_uva_error_t) {"'call' instruction set condition is invalid!","INVALID_CONDITION","value",lines[i]});
							continue;
						}
					}
					
					const vector<_uva_snippet_t> call_out = this->_transpile_(text,this->vir_regs_names,this->vir_regs);
					for(_uva_snippet_t snippet:call_out){
						this->output.push_back(snippet);
					}
					}
					else this->errors.push_back((_uva_error_t){"'call' instructions set arguments are invalid!","INVALID_VALUE","value",lines[i]}); 
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
