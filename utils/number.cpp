#include "./number.hpp"

uint32_t to_uint32(string text)
{
	uint32_t output = 0;

	for (uint8_t i = 0; i < text.length(); i++)
	{
		output += (text[i] - 48) * pow(10, i);
	}

	return output;
};

double to_double(string text)
{
	double output = 0;
	bool neg = text[0] == '-';

	const vector<string> parts = split(text, '.');

	for (int i = parts[0].length(); i != -1; i--)
	{
		const int dig = parts[0][i] - 48;
		if (dig < 0 || dig > 9)
			continue;
		output += dig * pow(10, parts[0].length() - i - 1);
	}

	if (parts.size() > 1)
	{
		for (int i = 0; i < parts[1].length() + 1; i++)
		{
			const int dig = parts[1][i] - 48;
			if (dig < 0 || dig > 9)
				continue;
			output += dig / pow(10, i + 1);
		}
	}

	return neg ? -output : output;
};

bool isValidNumber(string text)
{
	char symbols[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};

	for (char ch : text)
	{
		bool isNumber = false;
		for (char sym : symbols)
		{
			isNumber = (sym == ch || isNumber);
		}
		if (!isNumber)
			return false;
	}
	return true;
}