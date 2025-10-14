#include <string>
#include <vector>
#include <cstdint>
#include <vector>
#include "../utils/string.hpp"
#include "../utils/number.hpp"
#include "../utils/vector.hpp"

using std::string;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;

struct _GAMA_X_LINKER_label_t {
	string name;
	string text;
	uint32_t line;
};

struct _GAMA_X_LINKER_error_t {
	string title;
	string type;
	string description;
	long int line;
};

struct _GAMA_X_LINKER_file_t {
	string content;
	string name;
};

typedef bool *(_GAMA_X_LINKER_err_check_t) (vector<_GAMA_X_LINKER_error_t>, vector<string>, _GAMA_X_LINKER_file_t, uint32_t);

class _GAMA_X_LINKER_ {
	vector<_GAMA_X_LINKER_file_t> files;
	vector<vector<_GAMA_X_LINKER_label_t>> labels;
	vector<_GAMA_X_LINKER_error_t> errors;

	void _slice_labels_(_GAMA_X_LINKER_file_t file);
	void _check_errors_(string content, string filename);
	string _merge_pre_processors_(vector<_GAMA_X_LINKER_file_t> files);
	string _merge_instructions_(vector<_GAMA_X_LINKER_file_t> files);

	public:
		uint8_t status = 0;

		_GAMA_X_LINKER_();
		_GAMA_X_LINKER_(vector<_GAMA_X_LINKER_file_t> files);
		string merge();
};
