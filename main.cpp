#include <iostream>
#include "./core/assembler.hpp"
#include "./utils/file.hpp"
#define VERSION "V1.0.0"

using std::cout;

struct _file_binary_output_t {
	_uva_snippet_t *snippets;
	uint32_t length;
};

File *out_f;
File *in_f;

char empties[7] = {'\r','\t','\n','\b',' ','\a','\v'};

void print_err(string type, string title, string cause);
void safe_exit(int code);

int main(int argc, char *argv[]){
	File help_f("./help.txt");
	_uva_config_t config;

	string file_in;
	string file_out;

	bool binary = false;

	config.x_max = 255;
	config.y_max = 255;

	for(uint8_t i = 0; i<argc; i++){
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
			const string help_txt = help_f.read();
			if(help_f.status == ios::goodbit) cout<<help_f.read()<<"\n\n"<<VERSION<<"\n";
			else print_err("System","Help file","Failed to open help file to print!");
			return (int) (help_f.status == ios::goodbit);
		}
		else if(param == "-x" || param == "--x-max"){
			if(i == argc-1){
				print_err("User","Invalid argument","[-x, --x-max] flag value missed!");
				return 1;
			}
			else if(filter(string(argv[i+1]),empties,7).empty()){
				print_err("User","Invalid argument","[-x, --x-max] flag value isnt valid!");
				return 1;
			}
			config.x_max = to_uint32(string(argv[i+1]));
		}
		else if(param == "-y" || param == "--y-max"){
			if(i == argc-1){
				print_err("User","Invalid argument","[-y, --y-max] flag value missed!");
				return 1;
			}
			else if(filter(argv[i+1],empties,7).empty()){
				print_err("User","Invalid argument","[-y, --y-max] flag value isnt valid!");
				return 1;
			}
			config.y_max = to_uint32(string(argv[i+1]));
		}
		else if(file_in.empty()){
			file_in = param;
		}
		else if(file_out.empty()){
			file_out = param;
		}
		else {
			print_err("User","Extra arguments","extra positional argument recieved!");
			return 1;
		}
	}
	
	File in_file(file_in);
	File out_file(file_out);
	in_f = &in_file;
	out_f = &out_file;
	
	if(in_f->status) {
		print_err("File","Invalid file","failed to open input file!");
		return 1;
	}
	if(out_f->status) {
		print_err("File","Invalid file!","Faild to open output file!");
		return 1;
	}
	

	const string input = in_f->read();
	
	if(!in_f->ok()){
                print_err("File","File failure","Failed on reading from input file!");
                return 1;
        }

	UVA_ASSEMBLER uva_asm(input,config);

	const vector<_uva_snippet_t> snippet_out = uva_asm.compile();
	if(binary){
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
			const string output_l = join(vector<string>({to_string(s.x),to_string(s.y),to_string(s.v),to_string(s.t)}),",");
			output += output_l;
		}
		out_f->overwrite(output);
	}

	if(!out_f->ok()){
		print_err("File","File failure","Failed on writing to output file!");
		return 1;
	}
	
	const bool erroed = uva_asm.errors.size();
	return erroed;
}

void print_err(string type, string title, string cause){
	cout<<type<<" Error: "<<title<<"!\n\n";
	cout<<cause<<"\n";
}
