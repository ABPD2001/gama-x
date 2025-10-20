#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include "./core/linker.hpp"
#define VERSION "V1.0.0"

using std::fstream;
using std::cout;
using std::filesystem::exists;
using std::string;
using std::ios;
using std::getline;

void print_err(string type, string title, string cause);

int main(int argc, char *argv[]){
	vector<_GAMA_X_LINKER_file_t> _linker_files_;
	vector<string> files_in;
	string output;
	char empties[7] = {'\n',' ','\t','\b','\v','\r','\a'};

	for(uint8_t i = 0; i<argc; i++){
		const string param = string(argv[i]);
		if(param == "-v" || param == "--version"){
			cout<<VERSION<<"\n";
			return 0;
		}
		else if(param == "-h" || param == "--help"){
			fstream help_f;
			help_f.open("./help.txt",ios::in);
			if(!help_f.is_open()) {
				print_err("System","Help file failure","Failed to open help file to print!");
				return 1;
			}
			string temp_l;
			while(getline(help_f,temp_l)){
				cout<<temp_l<<"\n";
			}
			help_f.close();
			cout<<VERSION<<"\n";
			return 0;
		}
		else if(param == "-o" || param == "--output"){
			if(i == argc-1){
				print_err("User","Invalid flag value!","[-o, --output] flag value is invalid!");
				return 1;
			}
			else {
				i++;
				output = string(argv[i]);
			}
		}
		else files_in.push_back(string(argv[i]));
	}

	for(uint16_t i = 0; i<files_in.size(); i++){
		bool exit = false;
		if(!exists(files_in[i])) print_err("File","Invalid file","Input file "+files_in[i]+", not exists!");
		if(exit) return 1;

		fstream temp_f;
		temp_f.open(files_in[i],ios::in);
		string content;
		string temp_l;
		while(getline(temp_f,temp_l)){
			content+=temp_l+'\n';
		}
		content[content.length()-1] = '\0';
		temp_f.close();
		_linker_files_.push_back((_GAMA_X_LINKER_file_t) {content,files_in[i]});
	}

	_GAMA_X_LINKER_ linker(_linker_files_);
	const string merged_content = linker.merge();
	fstream out_f;
	out_f.open(output,ios::out);

	if(out_f.is_open()) out_f << merged_content;	
	else {
		print_err("File","Invalid file","Failed to open output file!");
		out_f.close();
		return 1;
	}
	out_f.close();
	

	return 0;
}

void print_err(string type, string title, string cause){
	cout<<type<<" Error: "<<title<<"!\n\n";
	cout<<cause<<"\n";
}
