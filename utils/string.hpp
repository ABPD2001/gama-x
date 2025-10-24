#ifndef StRING_HPP
#define STRING_HPP
#include <string>
#include <vector>
#include <cstdint>

using std::string;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;
using std::vector;

vector<string> split(string text, char splitter);
string join(vector<string> arr, string glue);
string trim(string text);
string replaceAll(string text, string from, string to);
string filter(string text, char from);
string filter(string text, char *chars, uint8_t count);
inline string filter(string text, string from)
{
	return replaceAll(text, from, "");
}
#endif