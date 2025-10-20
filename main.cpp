#include <iostream>
#include <fstream>
#include "./core/assembler.hpp"
#include <filesystem>
#define VERSION "V1.0.0"

using std::cout;
using std::fstream;
using std::ios;
using std::getline;
namespace fs = std::filesystem;

struct _file_binary_output_t {
	_uva_snippet_t *snippets;
	uint32_t length;
};

fstream out_f;
fstream in_f;

char empties[7] = {'\r','\t','\n','\b',' ','\a','\v'};

void print_err(string type, string title, string cause);
void safe_exit(int code);

int main(int argc, char *argv[]){
	fstream help_f;
	_uva_config_t config;

	string file_in;
	string file_out;

	help_f.open("./help.txt",ios::in);

	bool binary = false;

	for(uint8_t i = 1; i<argc; i++){
		const string param = string(argv[i]);
		
		if(param == "-b" || param == "--binary") binary = true;
		else if(param == "-v" || param == "--version") {
			cout<<VERSION<<"\n";
			return 0;
		}
		else if(param == "-i" || param == "--registers-count"){
			vector<string> values;
			vector<double> reg_values;
			
			if(i == argc-1) {
				print_err("User","Invalid argument","[-i, --register-initial] flag value missed!");
				return 1;
			}
			
			else if(filter(string(argv[i+1]),empties,7).empty()){
				print_err("User","Invalid arguemnt","[-i, --register-initial] flag value isnt valid!");
				return 1;
			}

			values = split(string(argv[i+1]), ',');
			
			for(string s:values){
				reg_values.push_back(to_double(s));
			}

			const uint16_t name_value_compare = config.reg_names.size() - config.reg_values.size();
			if(name_value_compare) {
				for(uint16_t i = 0; i<name_value_compare; i++){
					reg_values.push_back(0);
				}
			}

			config.reg_values = reg_values;	
		}
		else if(param == "-r" || param == "--registers-count"){
			if(i == argc-1){
				print_err("User","Invalid argument","[-r, --registers-count] flag value missed!");
				return 1;
			}
			else if(filter(string(argv[i+1]),empties,7).empty()){
				print_err("User","Invalid argument","[-r, --registers-count] flag value isnt valid!");
				return 1;
			}
			
			const int name_value_compare = config.reg_names.size() - config.reg_values.size();
			for(uint16_t i = 0; i<to_uint16(string(argv[i+1])); i++){
				config.reg_names.push_back("r"+to_string(i));
			}
			
			if(name_value_compare){
				for(uint16_t i = 0; i<name_value_compare; i++){
					config.reg_values.push_back(0);
				}
			}
		}
		else if(param == "-h" || param == "--help"){
			string help_txt = "";
			string help_temp;

			while(getline(help_f,help_temp)){
				help_txt += help_temp+'\n';
			}
			help_txt[help_txt.length()-1] = '\0';

			cout<<help_txt<<"\n\n"<<VERSION<<"\n";
			if (help_f.rdstate() != ios::goodbit) print_err("System","Help file","Failed to open help file to print!");
			const int output_code = (help_f.rdstate() != ios::goodbit);
			help_f.close();

			return output_code;
		}
		else if(file_in.empty()) file_in = param;
		else if(file_out.empty()) file_out = param;
		else {
			print_err("User","Extra arguments","extra positional argument recieved!");
			return 1;
		}
	}
	
	if(!fs::exists(file_in)){
		print_err("User","Invalid file path","path is not a valid and detectable file!");
		return 1;
	}

	in_f.open(file_in,ios::in);
	out_f.open(file_out,ios::out);
	
	if(in_f.rdstate() != ios::goodbit){
		print_err("File","File failure","Failed to open input file!");
		return 1;
	}

	string input;
	string input_temp;

	while(getline(in_f,input_temp)){
		input+=input_temp+'\n';
	}
	input[input.length()-1] = '\0';

	UVA_ASSEMBLER uva_asm(input,config);
	const vector<_uva_snippet_t> snippet_out = uva_asm.compile();
	if(binary){
		out_f.close();
		out_f.open(file_out,ios::out|ios::binary);
//		const uint32_t length = snippet_out.size();
//		_uva_snippet_t *bin_snippets;
//		bin_snippets = new _uva_snippet_t[length];
//		
//		for(uint32_t i = 0; i<length; i++){
//			bin_snippets[i] = snippet_out[i];
//		}
//
//		_file_binary_output_t binary_output = {bin_snippets,length};
//		out_f->file.open(out_f->path,ios::binary|ios::out);
//		out_f->file.write((char*) &binary_output, sizeof(binary_output));
//		out_f->file.close();
		
	}
	else {
		string output;
		for(_uva_snippet_t s:snippet_out){
			const string output_l = join(vector<string>({to_string(s.x),to_string(s.y),to_string(s.t)}),",")+'\n';
			output += output_l;
		}
		if(uva_asm.errors.size()){
			for(_uva_error_t err:uva_asm.errors){
				cout<<err.message<<"\n";
				cout<<err.cause<<"\n";
			}
		}
		output[output.size()-1] = '\0';
		out_f << output;
		out_f.close();
	}
	
	const bool erroed = uva_asm.errors.size();
	return erroed;
}

void print_err(string type, string title, string cause){
	cout<<type<<" Error: "<<title<<"!\n\n";
	cout<<cause<<"\n";
}
