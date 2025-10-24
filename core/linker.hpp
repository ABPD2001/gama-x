#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

using std::cout;
using std::string;
using std::uint32_t;
using std::vector;

struct _GXL_error_t
{
	string file;
	string message;
	string type;
};

struct _GXL_file_t
{
	string content;
	string name;
};

class _GXL_
{
	vector<_GXL_error_t> errors;
	vector<_GXL_file_t> files;
	string _merge_instructions_(vector<_GXL_file_t> files);
	string _merge_pre_processors_(vector<_GXL_file_t> files);

public:
	_GXL_();
	_GXL_(vector<_GXL_file_t> files);
	string merge();
};