#include <iostream>
#include <fstream>
#include "./core/assembler.hpp"
#include "./utils/binary.hpp"
#include <filesystem>
#define VERSION "V1.0.0"
#define HELP "<-- Gama-X Assembler -->\n\nUsage: gxa [*.s files...] [-o, -h, -v...]\nFlags:\n\t[-h, --help]: print this catalog.\n\t[-v, --version]: show version and exit.\n\t[-i, --initial-registers]: set initial value for registers [note: it can seperated by comma, also its dependent to -r flag].\n\t[-r, --registers-count]: set count of working registers, number cannot be negetive.\n\t[-b, --binary]: output binary instead of text.\n"

using std::cout;
using std::fstream;
using std::getline;
using std::ios;
namespace fs = std::filesystem;

fstream out_f;
fstream in_f;

char empties[7] = {'\r', '\t', '\n', '\b', ' ', '\a', '\v'};

void print_err(string type, string title, string cause);
void safe_exit(int code);

int main(int argc, char *argv[])
{
	fstream help_f;
	_GXA_config_t config;

	string file_in;
	string file_out;

	//help_f.open("./help.txt", ios::in);

	uint8_t binary = 0;

	for (uint8_t i = 1; i < argc; i++)
	{
		const string param = string(argv[i]);

		if (param == "-b" || param == "--binary"){
			if(i == argc-1){
				print_err("User","Invalid argument","[-b, --binary] flag value missed!");
				return 1;
			}
			else if(filter(string(argv[i+1]),empties,7).empty() || !isValidNumber(string(argv[i+1]))){
				print_err("User","Invalid argument","[-b, --binary] flag value isnt valid!");
				return 1;
			}
			binary = to_uint32(string(argv[i+1]));
			i++;
		}
		else if (param == "-v" || param == "--version")
		{
			cout << VERSION << "\n";
			return 0;
		}
		else if (param == "-i" || param == "--registers-count")
		{
			vector<string> values;
			vector<double> reg_values;

			if (i == argc - 1)
			{
				print_err("User", "Invalid argument", "[-i, --register-initial] flag value missed!");
				return 1;
			}

			else if (filter(string(argv[i + 1]), empties, 7).empty())
			{
				print_err("User", "Invalid arguemnt", "[-i, --register-initial] flag value isnt valid!");
				return 1;
			}

			values = split(string(argv[i + 1]), ',');

			for (string s : values)
			{
				reg_values.push_back(to_double(s));
			}

			const uint16_t name_value_compare = config.reg_names.size() - config.reg_values.size();
			if (name_value_compare)
			{
				for (uint16_t i = 0; i < name_value_compare; i++)
				{
					reg_values.push_back(0);
				}
			}

			config.reg_values = reg_values;
		}
		else if (param == "-r" || param == "--registers-count")
		{
			if (i == argc - 1)
			{
				print_err("User", "Invalid argument", "[-r, --registers-count] flag value missed!");
				return 1;
			}
			else if (filter(string(argv[i + 1]), empties, 7).empty())
			{
				print_err("User", "Invalid argument", "[-r, --registers-count] flag value isnt valid!");
				return 1;
			}

			const int name_value_compare = config.reg_names.size() - config.reg_values.size();
			for (uint16_t i = 0; i < to_uint16(string(argv[i + 1])); i++)
			{
				config.reg_names.push_back("r" + to_string(i));
			}

			if (name_value_compare)
			{
				for (uint16_t i = 0; i < name_value_compare; i++)
				{
					config.reg_values.push_back(0);
				}
			}
		}
		else if (param == "-h" || param == "--help")
		{
	//		string help_txt = "";
	//		string help_temp;

	//		while (getline(help_f, help_temp))
	//		{
	//			help_txt += help_temp + '\n';
	//		}
	//		help_txt[help_txt.length() - 1] = '\0';

			cout << HELP << VERSION << "\n";
	//		if (help_f.rdstate() != ios::goodbit)
	//			print_err("System", "Help file", "Failed to open help file to print!");
	//		const int output_code = (help_f.rdstate() != ios::goodbit);
	//		help_f.close();
	return 0;
	//		return output_code;
		}
		else if (file_in.empty())
			file_in = param;
		else if (file_out.empty())
			file_out = param;
		else
		{
			print_err("User", "Extra arguments", "extra positional argument recieved!");
			return 1;
		}
	}

	if (!fs::exists(file_in))
	{
		print_err("User", "Invalid file path", "path is not a valid and detectable file!");
		return 1;
	}

	in_f.open(file_in, ios::in);
	out_f.open(file_out, ios::out);

	if (in_f.rdstate() != ios::goodbit)
	{
		print_err("File", "File failure", "Failed to open input file!");
		return 1;
	}

	string input;
	string input_temp;

	while (getline(in_f, input_temp))
	{
		input += input_temp + '\n';
	}
	input[input.length() - 1] = '\0';

	_GXA_ gamaX_asm(input, config);
	const vector<_GXA_snippet_t> snippet_out = gamaX_asm.compile();
	if (binary)
	{
		out_f.close();
		out_f.open(file_out, ios::out);
		string file_out = "";
		for(unsigned int i = 0; i<snippet_out.size(); i++){
			string binx = to_binary(snippet_out[i].x);
			string biny = to_binary(snippet_out[i].y);
			string bint = to_binary64(snippet_out[i].t);

			const string binx_parts[4] = {binx.substr(0,8), binx.substr(8,8), binx.substr(16,8), binx.substr(24,8)};
			const string biny_parts[4] = {biny.substr(0,8), biny.substr(8,8), biny.substr(16,8), biny.substr(24,8)} ;
			const string bint_parts[8] =  {bint.substr(0,8), bint.substr(8,8), bint.substr(16,8), bint.substr(24,8), bint.substr(32,8), bint.substr(40,8), bint.substr(48,8),bint.substr(56,8)};
			for(uint8_t i = 0; i<binary; i++){
				file_out += from_binary8(binx_parts[3-i]);
			}
			for(uint8_t i = 0; i<binary; i++){
				file_out += (char) from_binary8(biny_parts[3-i]);
			}
			for (uint8_t i = 0; i<8; i++){
				file_out += (char) from_binary8(bint_parts[3-i]);
			}
		}
		out_f<<file_out<<"\n";
		out_f.close();
	}
	else
	{
		string output;
		for (_GXA_snippet_t s : snippet_out)
		{
			const string output_l = join(vector<string>({to_string(s.x), to_string(s.y), to_string(s.t)}), ",") + '\n';
			output += output_l;
		}
		if (gamaX_asm.errors.size())
		{
			for (_GXA_error_t err : gamaX_asm.errors)
			{
				cout << err.message << "\n";
				cout << err.cause << "\n";
			}
		}
		output[output.size() - 1] = '\0';
		out_f << output;
		out_f.close();
	}

	const bool erroed = gamaX_asm.errors.size();
	return erroed;
}

void print_err(string type, string title, string cause)
{
	cout << type << " Error: " << title << "!\n\n";
	cout << cause << "\n";
}
