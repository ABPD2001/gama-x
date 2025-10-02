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
using std::ios::iostate;
using uint8_t;

namespace fs = std::filesystem;

class File {
	fstream file;
	string path;

	void check_status();
	void init_status();
	
	public:
		iostate status = ios::goodbit;
		uint8_t status_code = 0;

		File(string path, bool binary = false);
		string read();
		iostate write(string content);
		iostate overwrite(string content);
		template<typename T> iostate bin_read(T *output, size_t size);
		template<typename T> iostate bin_write(T *content, size_t size);
		bool ok();
};
#endif
