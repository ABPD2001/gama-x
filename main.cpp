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
	bool verbose = false;

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

		else if(param == "-v" || param == "--verbose") verbose = true;
		else if(param == "-V" || param == "--version") {
			cout<<VERSION<<"\n";
			return 0;
		}
		else files.push_back(string(argv[i]));
	
	}

	if(!files.size()) {
		cout<<"No file entered!\n";
		return 1;
	}
	UVA_ASSEMBLER uva_asm = {files,output_file,verbose};
	if(verbose) cout<<"Preparing for compilation...\n";
	uva_asm.compile();

	if(verbose) cout<<"Compilation completed.\n";

	for(_uva_error_t err:uva_asm.errors){
		cout<<"Error at line" << err.idx<<": "<<err.message<<"\nCause: "<<err.cause<<"\n";
	}

	const bool errored = uva_asm.errors.size()>0;
	if(errored) cout<<"Compilation failed!\n";

	return (int) errored;
}
