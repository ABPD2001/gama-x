#include "./linker.hpp"

_GXL_::_GXL_(vector<_GXL_file_t> files)
{
	this->files = files;
}
_GXL_::_GXL_() {};

string _GXL_::_merge_pre_processors_(vector<_GXL_file_t> files)
{
	string output = "";

	for (_GXL_file_t f : files)
	{
		const short int idx = f.content.find(".end");

		if (idx == string::npos)
		{
			this->errors.push_back((_GXL_error_t){f.name, "Invalid pre-processor segmention!", "syntax"});
			continue;
		}
		int idx_module = f.content.find(".module");
		int idx_main = f.content.find(".main");

		if ((idx_main == string::npos && idx_module == string::npos) || (idx_main != string::npos && idx_module != string::npos))
		{
			this->errors.push_back((_GXL_error_t){f.name, "Invalid pre-processor type, its required to add main point or modular mode, only one mode!", "type"});
			continue;
		}

		string pre_processors = f.content.substr(0, idx);

		if (idx_module == string::npos)
			output += pre_processors + '\n';
		else
		{
			pre_processors.replace(idx_module, 7, string(""));
			output = pre_processors + '\n';
		}
	}

	output[output.length() - 1] = '\0';
	return output;
}

string _GXL_::_merge_instructions_(vector<_GXL_file_t> files)
{
	string output = "";
	for (_GXL_file_t f : files)
	{
		int idx = f.content.find(".end");

		if (idx == string::npos)
		{
			this->errors.push_back((_GXL_error_t){f.name, "Invalid instruction segmention (pre-processors invalid end point)!", "syntax"});
			continue;
		}
		cout << f.content.length() << "\n";
		const string instructions = f.content.substr(idx + 4, f.content.length() - idx - 4);
		// cout<<instructions<<"\n";
		output += instructions + '\n';
	}
	output[output.length() - 1] = '\0';
	return output;
}

string _GXL_::merge()
{
	return this->_merge_pre_processors_(this->files) + "\n.end\n" + this->_merge_instructions_(this->files);
}