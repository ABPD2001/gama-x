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
	_uva_config_t config;
	string output_path;

	bool binary = false;

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
			help_f.close();

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
		else if(param == "-b" || param == "--binary") binary = true;
		else if(param == "-i" || param == "--registers-initial"){
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
		else files_in.push_back(param);
	
	}
	for(string f:files_in){
		if(!fs::exists(f)){
			print_err("User","Invalid file path","File path ("+f+") isnt valid!");
			return 1;
		}
	}

	return 0;
}
