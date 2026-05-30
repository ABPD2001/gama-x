#include "./string.hpp"

string toBinary(long int value, uint8_t mode)
{
	string output;
	uint8_t ubytes[8];
	signed char sbytes[8];
	const uint8_t bytes = pow(2, (mode / 2));

	switch (mode)
	{
	case 0: // s64.
		for (uint8_t i = 0; i < 8; i++)
		{
			sbytes[i] = value & (0xff << (i * 8));
		}
		break;
	case 1: // u32.
		for (uint8_t i = 0; i < 4; i++)
		{
			ubytes[i] = ((uint32_t)value) & (0xff << (i * 8));
		}
		break;
	case 2: // s32
		for (uint8_t i = 0; i < 4; i++)
		{
			sbytes[i] = ((unsigned int)value) & (0xff << (i * 8));
		}
		break;
	case 3: // u16
		for (uint8_t i = 0; i < 2; i++)
		{
			ubytes[i] = ((uint16_t)value) & (0xff << (i * 8));
		}
		break;
	case 4: // s16
		for (uint8_t i = 0; i < 2; i++)
		{
			sbytes[i] = ((short int)value) & (0xff << (i * 8));
		}
		break;
	case 5: // u8
		ubytes[0] = ((uint8_t)value) & (0xff);
		break;
	case 6: // s8
		sbytes[0] = ((signed char)value) & (0xff);
		break;
	default:
		return "";
	}

	for (uint8_t i = 0; i < bytes; i++)
	{
		if (!mode % 2)
			output += sbytes[i];
		else
			output += ubytes[i];
	}

	return output;
}

bool is_valid_version(string text)
{
	const vector<string> parts = split(text, '.');
	if (parts.size() != 3)
		return false;
	if (!(isValidNumber(parts[0]) && isValidNumber(parts[1]) && isValidNumber(parts[2])))
		return false;
	if (parts[0].length() > 2 || parts[1].length() > 3 || parts[2].length() > 5)
		return false;
	return true;
}

vector<string> split(string text, char splitter)
{
	vector<string> output;
	string temp;

	for (char ch : text)
	{
		if (ch == splitter)
		{
			output.push_back(temp);
			temp.clear();
			continue;
		}
		temp += ch;
	}
	output.push_back(temp);
	temp.clear();

	return output;
}

vector<argument_t> parse_arguments(string content)
{
	vector<argument_t> output;
	const vector<string> lines = split(content, '\n');

	for (string l : lines)
	{
		for (uint32_t i = 0; i < l.length(); i++)
		{
			argument_t argument;
			bool skip = false;

			if (l[i] == '=')
			{
				const string key = trim(l.substr(0, i));
				const string value = trim(l.substr(i + 1, l.length()));

				for (argument_t arg : output)
				{
					if (arg.key == key)
					{
						arg.value = value;
						break;
					}
				}
				argument = {value, key};
			}
			if (skip)
				continue;
		}
	}

	return output;
}

string join(vector<string> arr, string glue)
{
	string output = "";
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		output += arr[i];
		if (i != arr.size() - 1)
			output += glue;
	}
	return output;
}

bool includes(char *arr, uint8_t count, char ch)
{
	for (uint8_t i = 0; i < count; i++)
	{
		if (arr[i] == ch)
			return true;
	}
	return false;
}

string trim(string text)
{
	string output = "";
	signed long int start = -1;
	signed long int end = -1;
	char empties[8] = {'\n', '\t', '\r', ' ', '\b', '\f', '\a', '\v'};

	for (uint32_t i = 0; i < text.length(); i++)
	{
		if (start == -1 && !includes(empties, 8, text[i]))
		{
			start = i;
			break;
		}
	}

	for (int i = text.length() - 1; i > -1; i--)
	{
		if (end == -1 && !includes(empties, 7, text[i]))
		{
			end = i;
			break;
		}
	}
	if (end == -1 || start == -1)
		return "";

	output = text.substr(start, end - start + 1);
	string cpy = "";
	bool spaced = false;

	for (uint32_t i = 0; i < output.length(); i++)
	{
		if (includes(empties, 7, output[i]))
		{
			if (!spaced)
			{
				spaced = true;
				cpy += output[i];
			}
			else
				continue;
		}
		else
		{
			spaced = false;
			cpy += output[i];
		}
	}

	return cpy;
}

string replaceAll(string text, string from, string to)
{
	string cpy = text;

	while (cpy.find(from) != string::npos)
	{
		cpy.replace(cpy.find(from), to.length(), to);
	}

	return cpy;
}

string filter(string text, char from)
{
	string output = "";
	for (char ch : text)
	{
		if (ch != from)
			output += ch;
	}
	return output;
}

string filter(string text, char *chars, uint8_t count)
{
	string output = "";
	for (char ch : text)
	{
		if (includes(chars, count, ch))
			continue;
		output += ch;
	}
	return output;
}