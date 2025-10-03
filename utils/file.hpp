#ifndef FILE_HPP
#define FILE_HPP
#include <fstream>
#include <string>
#include <cstdint>
#include <filesystem>

using std::string;
using std::fstream;
using std::getline;
using std::ios;
using std::uint8_t;

namespace fs = std::filesystem;

class File {
	bool check_status();
	public:
		fstream file;
		string path;
		uint8_t status = 0;

		File();
		File(string path);
		string read();
		uint8_t write(string content);
		uint8_t overwrite(string content);
		template<typename T> uint8_t bin_read(T *output, size_t size);
		template<typename T> uint8_t bin_write(T *content, size_t size);
		bool ok();
};
#endif
