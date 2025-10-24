#include <iostream>
#include <fstream>
#include <filesystem>
#include "./core/assembler.hpp"
#include "./core/linker.hpp"
#define VERSION "V1.0.0"

using std::cout;
using std::fstream;
using std::getline;
using std::ios;
namespace fs = std::filesystem;

struct _file_binary_output_t
{
	_GXA_snippet_t *snippets;
	uint32_t length;
};

fstream fout;
vector<fstream> files_in;

char empties[7] = {'\r', '\t', '\n', '\b', ' ', '\a', '\v'};

void print_err(string type, string title, string cause);
void safe_exit(int code);

int main(int argc, char *argv[])
{
	vector<_GXL_file_t> _linker_files_;
	_GXA_config_t config;
	fstream help_f;

	vector<string> files_in_path;
	string file_out;

	help_f.open("./help.txt", ios::in);

	bool binary = false;

	for (uint8_t i = 1; i < argc; i++)
	{
		const string param = string(argv[i]);

		if (param == "-b" || param == "--binary")
			binary = true;
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
		else if (param == "-o" || param == "--output")
		{
			if (i == argc - 1)
			{
				print_err("User", "Invalid output file argument!", "[-o, --output] flag value isnt valid!");
				return 1;
			}
			i++;
			file_out = string(argv[i]);
		}
		else if (param == "-h" || param == "--help")
		{
			string help_txt = "";
			string help_temp;

			while (getline(help_f, help_temp))
			{
				help_txt += help_temp + '\n';
			}
			help_txt[help_txt.length() - 1] = '\0';

			cout << help_txt << "\n\n"
				 << VERSION << "\n";
			if (help_f.rdstate() != ios::goodbit)
				print_err("System", "Help file", "Failed to open help file to print!");
			const int output_code = (help_f.rdstate() != ios::goodbit);
			help_f.close();

			return output_code;
		}
		else
			files_in_path.push_back(param);
	}

	for (uint16_t i = 0; i < files_in_path.size(); i++)
	{
		if (!fs::exists(files_in_path[i]))
		{
			print_err("File", "Invalid file path", "file path (" + files_in_path[i] + ") is not a valid and detectable file!");
			return 1;
		}
	}

	for (string p : files_in_path)
	{
		fstream file;
		string file_content;
		string file_temp;

		file.open(p, ios::in);
		files_in.push_back(file);

		while (getline(file, file_temp))
		{
			file_content += file_temp + '\n';
		}
		file_content[file_content.length() - 1] = '\0';

		_linker_files_.push_back((_GXL_file_t){file_content, p});
		file.close();
	}
	_GXL_ gamaX_linker(_linker_files_);
	_GXA_ gamaX_asm(gamaX_linker.merge(), config);

	const vector<_GXA_snippet_t> snippet_out = gamaX_asm.compile();
	if (binary)
	{
	}
	else
	{
		fout.open(file_out, ios::out);

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
		fout << output;
		fout.close();
	}

	const bool erroed = gamaX_asm.errors.size();
	return erroed;
}

void print_err(string type, string title, string cause)
{
	cout << type << " Error: " << title << "!\n\n";
	cout << cause << "\n";
}