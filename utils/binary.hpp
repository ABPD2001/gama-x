#ifndef BINARY_HPP
#define BINARY_HPP
#include <string>
#include <cstdint>
#include <cmath>
#include <iostream>

using std::string;
using std::uint64_t;
using std::uint32_t;
using std::uint8_t;
using std::to_string;
using std::pow;
using std::cout;

string to_binary64(uint64_t value);
string to_binary(uint32_t value);
uint64_t from_binary64(string value);
uint32_t from_binary32(string value);
uint8_t from_binary8(string value);
#endif
