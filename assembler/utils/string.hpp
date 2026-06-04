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

struct argument_t
{
	string value;
	string key;
};

vector<argument_t> parse_arguments(string content);

vector<string> split(string text, char splitter);
string join(vector<string> arr, string glue);
string trim(string text);
string replaceAll(string text, string from, string to);
string filter(string text, char from);
string filter(string text, char *chars, uint8_t count);
string toUppercase(string text);
string toLowercase(string text);

string toBinary(long int value, uint8_t mode);
inline string toBinaryu64(uint64_t value)
{
	string output;
	for (uint8_t i = 0; i < 8; i++)
	{
		output += (value & (0xff << (i * 8)));
	}
	return output;
}

inline string filter(string text, string from)
{
	return replaceAll(text, from, "");
}

inline string toUppercase(string text)
{
	string output;

	for (char ch : text)
	{
		if (ch >= 97 && ch <= 122)
		{
			output += ch - 32;
		}
	}

	return output;
}

inline string toLowercase(string text)
{
	string output;

	for (char ch : text)
	{
		if (ch >= 65 && ch <= 90)
		{
			output += ch + 32;
		}
	}

	return output;
}
#endif
