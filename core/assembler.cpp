#include "./assembler.hpp"

_GXA_::_GXA_(string content, _GXA_config_t config = {})
{
	this->input = content;
	this->config = config;
}
_GXA_::_GXA_() {}

vector<_GXA_snippet_t> _GXA_::_transpile_(string content, uint16_t line_idx)
{
	vector<_GXA_snippet_t> output;
	const vector<string> &vir_regs_names = this->vir_regs_names;
	vector<double> &vir_regs = this->vir_regs;
	vector<string> lines = split(content, '\n');
	string temp_line;

	signed long int cmp = 0;
	char empties[7] = {'\t', '\n', '\f', '\r', ' ', '\a', '\v'};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		if (!filter(lines[i], empties, 7).length())
			continue;

		temp_line = trim(lines[i]);
		const vector<string> parts = split(temp_line, ' ');
		vector<string> params = parts.size() > 1 ? split(join(vector<string>(parts.begin() + 1, parts.end()), " "), ',') : vector<string>();
		const uint32_t lx = i + line_idx;
		cout << params.size() << " " << CMD << "\n";

		for (uint8_t i = 0; i < params.size(); i++)
		{
			if (filter(params[i], empties, 7).empty())
				this->errors.push_back((_GXA_error_t){"'" + CMD + "' instruction set arguemnt " + to_string(i + 1) + " has invalid syntax!", "INVALID_VALUE_SYNTAX", "syntax", lx});
		}

		if (CMD == "mov")
		{
			signed short int tar_reg_idx = -1;
			if (params.size() != 2)
			{
				this->errors.push_back((_GXA_error_t){"'mov' instrction set arguments missed!", "MISSED_ARGUMENS", "value", lx});
				continue;
			}

			tar_reg_idx = find(vir_regs_names, params[0]);
			if (tar_reg_idx != -1 && params[1].length())
			{
				if (params[1][0] == '#')
					vir_regs[tar_reg_idx] = to_double(trim(params[1]));
				else
				{
					signed short int reg2 = find(vir_regs_names, params[1]);
					vir_regs[tar_reg_idx] = vir_regs[reg2];
				}
			}
			else
				this->errors.push_back((_GXA_error_t){"'mov' instruction set value missed or is invalid!", "MISSED_OR_INVALID_REGISTER_NAME", "value", lx});
		}
		else if (CMD == "log")
		{
			signed short int tar_reg_idx = -1;
			uint32_t bin1;
			uint32_t bin2;

			if (params.size() != 3)
			{
				this->errors.push_back((_GXA_error_t){"'log' instruction set value missed or is invalid!", "MISSED_OR_INVALID_REGISTER_NAME", "value", lx});
				continue;
			}
			tar_reg_idx = find(vir_regs_names, params[0]);
			if (tar_reg_idx > -1 && params[1].length())
			{
				bin1 = (uint32_t)vir_regs[tar_reg_idx];
				const uint8_t dest_idx = find(vir_regs_names, params[0]);

				if (params[1][0] == '#')
					bin2 = to_uint32(trim(params[1].substr(1, params[1].length())));
				else
				{
					tar_reg_idx = find(vir_regs_names, params[1]);
					if (tar_reg_idx > -1)
						bin2 = (uint32_t)(vir_regs[tar_reg_idx]);
				}
				cout << "values " << bin1 << " " << bin2 << "\n";
				if (params[2] == "AND")
					vir_regs[dest_idx] = bin1 & bin2;
				else if (params[2] == "ORR")
					vir_regs[dest_idx] = bin1 | bin2;
				else if (params[2] == "EOR")
					vir_regs[dest_idx] = bin1 ^ bin2;
				else if (params[2] == "NOR")
					vir_regs[dest_idx] = ~(bin1 | bin2);
				else if (params[2] == "NAND")
					vir_regs[dest_idx] = ~(bin1 & bin2);
				else if (params[2] == "NEOR")
					vir_regs[dest_idx] = (bin1 ^ bin2);
				else if (params[2] == "NOT")
					vir_regs[dest_idx] = ~bin1;
				else
				{
					this->errors.push_back((_GXA_error_t){"'log' instruction set operation is invalid!", "INVALID_LOGICAL_OPERATION", "value", lx});
					continue;
				}
			}
			else
			{
				this->errors.push_back((_GXA_error_t){"'log' instruction set arguments are invalid!", "INVALID_ARGUMENTS", "value", lx});
				continue;
			}
		}
		else if (CMD == "shf")
		{
			signed short int tar_reg_idx = -1;
			uint32_t val;
			uint32_t amount;

			if (params.size() != 3)
			{
				this->errors.push_back((_GXA_error_t){"'shf' instruction set arguments are invalid!", "INVALID_ARGUMENTS", "value", lx});
				continue;
			}
			else
			{
				tar_reg_idx = find(vir_regs_names, params[0]);
				if (tar_reg_idx > -1)
					val = vir_regs[tar_reg_idx];
				else
				{
					this->errors.push_back((_GXA_error_t){"'shf' instruction set argument is invalid!", "INVALID_REGISTERS_ARGUMENTS", "value", lx});
					continue;
				}
				if (params[1].size())
				{
					if (params[1][0] == '#')
						amount = to_uint32(trim(params[1].substr(1, params[1].length())));
					else
					{
						tar_reg_idx = find(vir_regs_names, params[1]);
						if (tar_reg_idx > -1)
							amount = (uint32_t)vir_regs[tar_reg_idx];
						else
						{
							this->errors.push_back((_GXA_error_t){"'shf' instruction set argument is invalid!", "INVALID_REGUSTERS_ARGUMENTS", "value", lx});
							continue;
						}
					}
				}
				tar_reg_idx = find(vir_regs_names, params[0]);
				cout << "shf --> " << val << " " << amount << '\n';
				if (params[2] == "LEFT")
					vir_regs[tar_reg_idx] = val << amount;
				else if (params[2] == "RIGHT")
					vir_regs[tar_reg_idx] = val >> amount;
				else
				{
					this->errors.push_back((_GXA_error_t){"'shf' instruction set operation is invalid!", "INVALID_SHIFT_OPERATION", "value", lx});
					continue;
				}
			}
		}
		else if (CMD == ".transpile")
		{
			_GXA_snippet_t snippet = {(uint64_t)vir_regs[2], (uint32_t)(vir_regs[0]), (uint32_t)(vir_regs[1])};
			output.push_back(snippet);
		}
		else if (CMD == ".reset")
		{
			for (uint16_t i = 0; i < this->vir_regs.size(); i++)
			{
				this->vir_regs[i] = 0;
			}
		}
		else if (CMD == "jmp")
		{
			if (params.size() > 2 || params.size() < 1)
				this->errors.push_back((_GXA_error_t){"'jmp' instruction set must have line index to jump!", "MISSED_VALUE", "syntax", lx});
			else if (isValidNumber(params[0]))
			{
				const uint32_t num = to_uint32(params[0]);
				if (num > lines.size() - 1 || num < 0)
				{
					this->errors.push_back((_GXA_error_t){"'jmp' instruction set argument is over/under of current block!", "INVALID_JMP_PLACE", "value", lx});
					continue;
				}
				if (params.size() == 2)
				{
					if (params[1] == "EQ" && cmp)
						continue;
					else if (params[1] == "LE" && cmp >= 0)
						continue;
					else if (params[1] == "GE" && cmp <= 0)
						continue;
					else if (params[1] != "AL")
					{
						this->errors.push_back((_GXA_error_t){"'jmp' instruction set condition is invalid!", "INVALID_CONDITION", "value", lx});
						continue;
					}
				}
				i = min(lines.size() - 1, num);
			}
			else
				this->errors.push_back((_GXA_error_t){"'jmp' instruction set jump address has invalid syntax!", "INVALID_ARGUEMNT_SYNTAX", "syntax", lx});
		}
		else if (CMD == "cmp")
		{
			if (params.size() != 2)
			{
				this->errors.push_back((_GXA_error_t){"'cmp' instruction set arguments are missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			long int reg1_idx;
			long int reg2_idx;

			if (params[0][0] == '#')
				reg1_idx = to_double(params[0]);
			else
			{
				reg1_idx = find(this->vir_regs_names, params[0]);
				if (reg1_idx == -1)
				{
					this->errors.push_back((_GXA_error_t){"'cmp' instruction set first argument isnt valid!", "INVALID_ARGUMENT", "value", lx});
					continue;
				}
			}
			if (params[1][0] == '#')
			{
				reg2_idx = to_double(params[1]);
				cout << reg1_idx << "-" << reg2_idx << "\n";
				cmp = reg1_idx - reg2_idx;
			}
			else
			{
				reg2_idx = find(this->vir_regs_names, params[1]);
				if (reg2_idx == -1)
				{
					this->errors.push_back((_GXA_error_t){"'cmp' instruction set second argument isnt valid!", "INVALID_ARGUMENT", "value", lx});
					continue;
				}
				cmp = vir_regs[reg1_idx] - vir_regs[reg2_idx];
			}
		}
		else if (CMD == "add")
		{
			if (params.size() != 2)
			{
				this->errors.push_back((_GXA_error_t){"'add' instructions set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			const signed short int reg_idx = find(vir_regs_names, params[0]);
			signed long int reg2 = 0;

			if (params[1][0] == '#')
				reg2 = to_double(params[1].substr(1, params[1].length()));
			else
				reg2 = vir_regs[find(vir_regs_names, params[1])];

			if (reg_idx > -1)
				vir_regs[reg_idx] += reg2;
			else
				this->errors.push_back((_GXA_error_t){"'add' instruction set arguements are invalid!", "INVALID_ARGUMENTS", "value", lx});
		}

		else if (CMD == "incr")
		{
			if (params.size() != 1)
			{
				this->errors.push_back((_GXA_error_t){"'incr' instructions set arguments are missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			const signed short int reg_idx = find(vir_regs_names, params[0]);
			if (reg_idx > -1)
				vir_regs[reg_idx]++;
			else
				this->errors.push_back((_GXA_error_t){"'incr' instructions set argmuents are invalid!", "INVALID_ARGUMENTS", "value", lx});
		}

		else if (CMD == "sub")
		{
			if (params.size() != 2)
			{
				this->errors.push_back((_GXA_error_t){"'min' instructions set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}

			const signed short int reg_idx = find(vir_regs_names, params[0]);
			signed long int reg2 = 0;

			if (params[1][0] == '#')
				reg2 = to_double(params[1].substr(1, params[1].length()));
			else
				reg2 = vir_regs[find(vir_regs_names, params[1])];

			if (reg_idx > -1)
				vir_regs[reg_idx] -= reg2;
			else
				this->errors.push_back((_GXA_error_t){"'add' instruction set arguments are invalid!", "INVALID_ARGUMENTS", "value", lx});
		}

		else if (CMD == "decr")
		{
			if (params.size() != 1)
			{
				this->errors.push_back((_GXA_error_t){"'decr' instructions set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			else if (params[0].length())
			{
				const signed short int reg_idx = find(vir_regs_names, params[0]);
				if (reg_idx > -1)
					vir_regs[reg_idx]--;
				else
					this->errors.push_back((_GXA_error_t){"'decr' instructions set arguments is invalid!", "INVALID_ARGUMENT", "value", lx});
			}
		}
		else if (CMD == "push")
		{
			if (params.size() != 1)
			{
				this->errors.push_back((_GXA_error_t){"'push' instruction set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			if (params[0][0] == '#')
				this->vir_stack.push_back(to_double(trim(params[0])));
			else
			{
				const short int reg = find(this->vir_regs_names, params[0]);
				if (reg > -1)
					this->vir_stack.push_back(this->vir_regs[reg]);
				else
					this->errors.push_back((_GXA_error_t){"'push' instruction set register is invalid!", "INVALID_STACK_REGISTER", "value", lx});
			}
		}
		else if (CMD == "pull")
		{
			if (params.size() != 1)
			{
				this->errors.push_back((_GXA_error_t){"'pull' instruction set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			const short int reg = find(this->vir_regs_names, params[0]);
			if (reg > -1)
			{
				this->vir_regs[reg] = this->vir_stack[this->vir_stack.size() - 1];
				this->vir_stack = vector<double>(this->vir_stack.begin(), this->vir_stack.end() - 1);
			}
			else
				this->errors.push_back((_GXA_error_t){"'pull' instruction set argument is invalid!", "INVALID_ARGUMENT", "value", lx});
		}
		else if (CMD == "call")
		{
			if (params.size() < 1 && params.size() > 3)
			{
				this->errors.push_back((_GXA_error_t){"'call' instructions set arguments missed!", "MISSED_ARGUMENTS", "value", lx});
				continue;
			}
			string text;
			for (uint16_t i = 0; i < this->labels.size(); i++)
			{
				if (this->labels[i].name == params[0])
					text = this->labels[i].text;
			}

			if (text.length())
			{
				if (params.size() == 2)
				{
					if (params[1] == "EQ" && cmp != 0)
						continue;
					else if (params[1] == "LE" && cmp >= 0)
						continue;
					else if (params[1] == "GE" && cmp <= 0)
						continue;
					else if (params[1] == "NE" && !cmp)
						continue;
				}

				const vector<_GXA_snippet_t> call_out = this->_transpile_(text, lx);
				for (uint32_t i = 0; i < call_out.size(); i++)
				{
					output.push_back(call_out[i]);
				}
			}
			else
				this->errors.push_back((_GXA_error_t){"'call' instructions set arguments are invalid!", "INVALID_VALUE", "value", lx});
		}
		else if (CMD == "nop")
		{
			for (string p : params)
			{
				for (int i = 0; i < to_double(p); i++)
				{
				}
			}
		}
		else
			this->errors.push_back((_GXA_error_t){"Invalid instruction entered!", "INVALID_INSTRUCTION", "decode", lx});
	}

	return output;
}

void _GXA_::_pre_processors_(string &content)
{
	vector<string> lines = split(content, '\n');
	char empties[7] = {'\r', '\n', ' ', '\a', '\b', '\t', '\f'};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		if (!filter(lines[i], empties, 7).length())
			continue;
		const vector<string> parts = split(lines[i], ' ');

		if (CMD == ".replace")
		{
			if (parts.size() != 3 || parts[1].empty() || parts[2].empty())
				this->errors.push_back((_GXA_error_t){"'.replace' pre-processor set arguments missed!", "CO_PROCCESSOR_VALUES_MISSED", "syntax", i});
			else
			{
				content = replaceAll(content, parts[1], parts[2]);
				lines = split(content, '\n');
				lines[i] = "_NONE_";
			}
		}
		else if (CMD == ".end")
		{
			lines[i] = "_NONE_";
			string out = "";
			for (string s : lines)
			{
				if (s == "_NONE_")
					continue;
				out += s + '\n';
			}
			content = out + '\n';
			break;
		}

		else if (CMD == ".main")
		{
			if (parts.size() != 2 || parts[1].empty())
				this->errors.push_back((_GXA_error_t){"'.main' pre-proccessor set argument missed!", "CO_PROCCESSOR_VALUE_MISSED", "syntax", i});
			else
			{
				this->startLabel = parts[1];
				lines[i] = "_NONE_";
			}
		}
	}
}

vector<_GXA_label_t> _GXA_::_slice_labels_(string content)
{
	vector<_GXA_label_t> output;
	vector<string> lines = split(content, '\n');
	string cur_label = "";

	int idx_line = -1;
	char empties[7] = {'\r', '\n', ' ', '\t', '\a'};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		if (!filter(lines[i], empties, 7).length())
			continue;
		lines[i] = trim(lines[i]);
		if (cur_label.empty() && lines[i].find(':') != string::npos && lines.size() - 1 != i)
		{
			cur_label = filter(filter(lines[i], ':'), empties, 7);
			for (_GXA_label_t lbl : this->labels)
			{
				if (lbl.name == cur_label)
				{
					this->errors.push_back((_GXA_error_t){"label " + cur_label + " overloaded!", "LABEL_OVERLOADED", "syntax", i});
					break;
				}
			}
			idx_line = i;
		}

		else if (lines[i].substr(0, 3) == "end" && cur_label.length() && idx_line != -1)
		{
			const string content = join(vector<string>(lines.begin() + idx_line + 1, lines.begin() + i), "\n");
			_GXA_label_t label = {content, cur_label, (uint32_t)idx_line};
			cur_label.clear();
			output.push_back(label);
		}
	}
	return output;
}

_GXA_::~_GXA_()
{
	this->status = 3;
	this->errors = vector<_GXA_error_t>();
	this->labels = vector<_GXA_label_t>();
	this->input.clear();
	this->startLabel.clear();
	this->vir_regs_names = vector<string>();
	this->vir_regs = vector<double>();
}

vector<_GXA_snippet_t> _GXA_::compile()
{
	vector<_GXA_snippet_t> out;
	string pre_processed_input = this->input;

	this->_pre_processors_(pre_processed_input);
	this->labels = this->_slice_labels_(pre_processed_input);
	bool found = false;
	for (_GXA_label_t lbl : this->labels)
	{
		if (lbl.name == this->startLabel)
		{
			const vector<_GXA_snippet_t> transpile_out = this->_transpile_(lbl.text, lbl.line_idx);
			for (_GXA_snippet_t snip : transpile_out)
			{
				out.push_back(snip);
			}
			found = true;
			break;
		}
	}

	if (!found)
		this->status = 2;

	return out;
}