#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include "./utils/number.hpp"
#include "./utils/string.hpp"
#define HELP "<-- UVision exporter -->\n\tBasic Usage: uve [input, output, -v,-h...]\n\t[-v, --version]: show version.\n\t[-h, --help]: print this catalog."
#define VERSION "V1.0.0"

using std::cout;
using std::cin;
using std::getline;
using std::string;
using std::to_string;
using std::fstream;
using std::ios;
using std::uint8_t;

void print_err(string type, string title, string desc);
void input(string ask,string &save);
int main(int argc, char *argv[]){
	string input_file;
	string output_file;

	for(uint8_t i = 1; i<argc; i++){
		string param = string(argv[i]);

		if(param == "-v" || param == "--version"){
			cout<<VERSION<<"\n";
			return 0;
		}
		else if(param == "-h" || param == "--help"){
			cout<<HELP<<"\n"<<VERSION<<"\n";
			return 0;
		}
		else if(input_file.empty()) input_file = param;
		else if(output_file.empty()) output_file = param;
		else {
			print_err("User","Extra argument","Extra arguemnt: "+param+" given!");
			return 1;
		}
	}
	string first_line = "{'name':'";
	string temp;
	input("Enter name:",temp);
	first_line += temp+"','description':'";
	input("Enter description:",temp);
	first_line += temp+"','date':'";
	input("Enter date:",temp);
	first_line += temp+"','time':";

	fstream input_f;
	input_f.open(input_file,ios::in);

	unsigned int maximum_time = 0;
	string content;
	while(getline(input_f,temp)){
		content+=temp+'\n';
		const vector<string> parts = split(temp,',');
		maximum_time+=to_uint32(parts[2]);
	}
	first_line+=to_string(maximum_time)+"}";
	content[content.length()-1] = '\0';
	first_line = replace_all(first_line,(char) 39, (char) 34);
	input_f.close();
	fstream output_f;
	output_f.open(output_file,ios::out);
	output_f<<first_line<<'\n'<<content;
	output_f.close();
	
	return 0;
}

void input(string ask, string& save){
	cout<<ask<<" ";
	getline(cin,save);
}

void print_err(string type, string title, string line){
	cout<<type<<" Error: "<<title<<"!\n"<<line<<"\n";
}
