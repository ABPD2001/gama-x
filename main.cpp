#include <iostream>
#include <string>
#include <iostream>
#include "./libs/assembler.hpp"
#include <fstream>
#include <filesystem>

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
	if(argv[i] == "-o" || argv[i] == "--output") {
		if(i == argc-1) {
			cout<<"flag '-o' value missed!\n";
			return 1;
		}
		output_file = argv[i+1];
		i++;
	}
	
	else if(argv[i] == "-h" || argv[i] == "--help") {
		cout<<help<<"\n";	
		return 0;
	}

	else if(argv[i] == "-v" || argv[i] == "--verbose") verbose = true;
	else files.push_back(argv[i]);
	cout<<argv[i]<<"\n";
	
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
