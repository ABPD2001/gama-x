#include <iostream>
#include <string>
#include <iostream>
#include "./core/assembler.hpp"
#include <fstream>
#include <filesystem>
#include <cmath>
#define VERSION "V1.0.0"

using std::string;
using std::cout;
using std::cin;
using std::getline;
using std::ios;

namespace fs = std::filesystem;

int main(int argc, char *argv[]){
	string output_file;
	string help = "";
	string temp_line;
	fstream help_f;
	_uva_config_t config;

	help_f.open("./help.txt",ios::in);

	while (getline(help_f,temp_line)){
		help+=temp_line+'\n';
	}

	temp_line.clear();
	vector<string> files;

	for(uint8_t i = 1; i<argc; i++){
		const string param = argv[i];
		if(param == "-o" || param == "--output") {
			if(i == argc-1) {
				cout<<"flag '-o' value missed!\n";
				return 1;
			}
			output_file = string(argv[i+1]);
			i++;
		}
	
		else if(param == "-h" || param == "--help") {
			cout<<help;	
			return 0;
		}
		else if(param == "-v" || param == "--version") {
			cout<<VERSION<<"\n";
			return 0;
		}
		else if(param == "-l" || param == "--register-length"){
			if(i == argc-1) {
				cout<<param<<" value missed!\n";
				return 1;
			}
			vector<string> reg_names;
			for(uint16_t i = 0; i<to_double(string(argv[i+1])); i++){
				reg_names.push_back('r'+to_string(i));
			}
			config.names = reg_names;
		}
		else if(param == "-i" || param == "--registers-initial"){
			if(i == argc-1){
				cout<<param<<" value missed!\n";
				return 1;
			}	
			vector<double> reg_values;
			const vector<string> values = split(param,',');
			for(uint16_t i = 0; i<values.size() && i<config.names.size(); i++){
				reg_values.push_back(to_double(values[i]));
			}
			config.values = reg_values;
		}
		else if(param == "-b" || param == "--binary"){
			config.binary = true;
		}
		else files.push_back(string(argv[i]));
	
	}

	if(!files.size()) {
		cout<<"No file entered!\n";
		return 1;
	}
	UVA_ASSEMBLER uva_asm = {files,output_file};
	uva_asm.compile();


	for(_uva_error_t err:uva_asm.errors){
		cout<<"Error at line" << err.idx<<": "<<err.message<<"\nCause: "<<err.cause<<"\n";
	}

	const bool errored = uva_asm.errors.size()>0;
	if(errored) cout<<"Compilation failed!\n";

	return (int) errored;
}
