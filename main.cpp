#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "./core/linker.hpp"
#include "./core/assembler.hpp"
#define VERSION "1.0.0"

using std::vector;
using std::string;
using std::cout;
using std::string;
using std::ios;
using std::uint8_t;

vector<fstream> files_in;
fstream file_out;

void print_err(string type, string title,string cause);

int main(int argc, char *argv[]){
	vector<string> input_files;
	string output_path;
	
	for(uint8_t i = 1; i<argc; i++){
		const string param = string(argv[i]);
		if(param == "-h" || param == "--help"){
			fstream help_f;
			help_f.open("./help.txt",ios::in);
			string help_temp;

			while(getline(help_temp,help_f)){
				cout<<help_temp<<"\n";
			}
			cout<<VERSION<<"\n";
			
			return 0;

		}
		else if(param == "-v" || param == "--version"){
			cout<<VERSION<<'\n';

			return 0;
		}
		else if(param == "-o" || param == "--output"){
			if(i == argc-1){
				print_err("User","Invalid output file!","[-o, --output] flag value isnt valid!");
				return 1;
			}
			i++;
			output_path = string(argv[i]);
			continue;
		}
	
	}

	return 0;
}
