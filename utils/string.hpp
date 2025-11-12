#ifndef STRING_HPP
#define STRING_HPP
#include <vector>
#include <string>

using std::vector;
using std::string;

vector<string> split(string text, char splitter);
string replace_all(string text, char from,char to);
#endif
