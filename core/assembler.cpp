#include "./assembler.hpp"

_GXA_::_GXA_(string content, _GXA_config_t config = {})
{
    this->input = content;
    this->config = config;
    if (this->config.register_values.size())
        this->vir_regs = this->config.register_values;
    if (this->config.registers_name.size())
        this->vir_regs_names = this->config.registers_name;
    if (this->config.special_registers_name.size())
        this->special_vir_regs_name = this->config.special_registers_name;
}
void _GXA_::init(string content, _GXA_config_t config = {})
{
    this->input = content;
    this->config = config;
    if (this->config.register_values.size())
        this->vir_regs = this->config.register_values;
    if (this->config.registers_name.size())
        this->vir_regs_names = this->config.registers_name;
    if (this->config.special_registers_name.size())
        this->special_vir_regs_name = this->config.special_registers_name;
}

_GXA_::_GXA_() {}
_GXA_::~_GXA_()
{
    this->status = 3;
    this->runtime_errors.clear();
    this->labels.clear();
    this->input.clear();
    this->startLabel = {};
    this->vir_regs_names.clear();
    this->special_vir_regs_name.clear();
    this->vir_regs.clear();
}

vector<_GXA_external_argument_t> _GXA_::_parse_argular_(string path, uint32_t index)
{
    fstream extern_f;
    vector<string> lines;
    string line;

    extern_f.open(path, ios::in);

    if (!extern_f.is_open())
    {
        _GXLT_error_cpy_t error = {path, "Failed to open file in '" + path + "'!", "FILE_READ_FAILURE", "input-output", index};
        this->runtime_errors.push_back(error);
        return;
    }
    while (getline(extern_f, line))
    {
        lines.push_back(line);
    }

    for (uint16_t i = 0; i < lines.size(); i++)
    {

        for (uint64_t j = 0; j < lines[i].length(); j++)
        {
            if (lines[i][j] == '=')
            {
                const string value = lines[i].substr(i + 1);
                const string name = lines[i].substr(0, i);

                if (!isValidNumber(value))
                {
                    _GXLT_error_cpy_t error = {path, "Invalid syntax for" + name + "arguments value!", "INVALID_ARGUMNET_VALUE", "input-output", index};
                    this->runtime_errors.push_back(error);
                    return;
                }

                _GXA_external_argument_t argument = {name, value};
                this->external_arguments.push_back(argument);
            }
        }
    }
}

void _GXA_::_open_extern_(string path, string name, uint32_t index)
{
    fstream extern_f;
    string content;
    string line;

    extern_f.open(path, ios::in);

    if (!extern_f.is_open())
    {
        _GXLT_error_cpy_t error = {path, "Failed to open file in '" + path + "'!", "FILE_READ_FAILURE", "input-output", index};
        this->runtime_errors.push_back(error);
        return;
    }
    while (getline(extern_f, line))
    {
        content += line + '\n';
    }

    content = content.substr(0, content.length() - 1);

    _GXA_external_t external = {content, name};
    this->externals.push_back(external);
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

void _GXA_::_pre_processors_(string &content, vector<uint32_t> ignore_lines)
{
    vector<string> lines = split(content, '\n');
    char empties[7] = {'\r', '\n', ' ', '\a', '\b', '\t', '\f'};

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        if (includes(ignore_lines, i))
            continue;
        if (!filter(lines[i], empties, 7).length())
            continue;
        const vector<string> parts = split(lines[i], ' ');

        if (parts[0] == ".replace")
        {
            content = replaceAll(content, parts[1], parts[2]);
            lines = split(content, '\n');
            lines[i] = "_NONE_";
        }
        else if (parts[0] == ".end")
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

        else if (parts[0] == ".main")
        {
            this->startLabel.name = parts[1];
            lines[i] = "_NONE_";
        }
        else if (parts[0] == ".argular")
        {
            vector<_GXA_external_argument_t> arguments = this->_parse_argular_(parts[1].substr(parts[1].find_first_of('"'), parts[1].find_last_of('"') - 1), i);
            this->external_arguments.insert(this->external_arguments.end(), arguments.begin(), arguments.end());
        }

        else if (parts[0] == ".extern")
        {
            this->_open_extern_(parts[1].substr(parts[1].find_first_of('"'), parts[1].find_last_of('"') - 1), parts[2], i);
        }
    }
}

vector<_GXA_snippet_t> _GXA_::_transpile_(string content, uint64_t line_idx)
{
    vector<_GXA_snippet_t> output;
    vector<string> lines = split(content, '\n');
    string temp_line;

    long int cmp = 0;
    char empties[7] = {'\t', '\n', '\f', '\r', ' ', '\a', '\v'};

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        if (!filter(lines[i], empties, 7).length())
            continue;

        temp_line = trim(lines[i]);
        const vector<string> parts = split(temp_line, ' ');
        vector<string> params = parts.size() > 1 ? split(join(vector<string>(parts.begin() + 1, parts.end()), " "), ',') : vector<string>();
        const uint32_t lx = i + line_idx;

        if (parts[0] == "mov")
        {
            short int tar_reg_idx = -1;

            tar_reg_idx = find(this->vir_regs_names, params[0]);
            if (params[1][0] == '#')
                this->vir_regs[tar_reg_idx] = to_autoNumber(trim(params[1]));
            else
            {
                short int reg2 = find(this->vir_regs_names, params[1]);
                this->vir_regs[tar_reg_idx] = this->vir_regs[reg2];
            }
        }
        else if (parts[0] == "log")
        {
            short int tar_reg_idx = -1;
            uint32_t bin1;
            uint32_t bin2;

            tar_reg_idx = find(this->vir_regs_names, params[0]);
            bin1 = (uint32_t)this->vir_regs[tar_reg_idx];
            const uint8_t dest_idx = find(this->vir_regs_names, params[0]);

            if (params[1][0] == '#')
                bin2 = to_uint32(trim(params[1].substr(1, params[1].length())));
            else
            {
                tar_reg_idx = find(this->vir_regs_names, params[1]);
                if (tar_reg_idx > -1)
                    bin2 = (uint32_t)(this->vir_regs[tar_reg_idx]);
            }
            if (params[2] == "AND")
                this->vir_regs[dest_idx] = bin1 & bin2;
            else if (params[2] == "ORR")
                this->vir_regs[dest_idx] = bin1 | bin2;
            else if (params[2] == "XOR")
                this->vir_regs[dest_idx] = bin1 ^ bin2;
            else if (params[2] == "NOR")
                this->vir_regs[dest_idx] = ~(bin1 | bin2);
            else if (params[2] == "NAND")
                this->vir_regs[dest_idx] = ~(bin1 & bin2);
            else if (params[2] == "NXOR")
                this->vir_regs[dest_idx] = (bin1 ^ bin2);
            else if (params[2] == "NOT")
                this->vir_regs[dest_idx] = ~bin1;
        }
        else if (parts[0] == "shf")
        {
            short int tar_reg_idx = -1;
            uint32_t val;
            uint32_t amount;

            tar_reg_idx = find(this->vir_regs_names, params[0]);
            val = this->vir_regs[tar_reg_idx];
            if (params[1].size())
            {
                if (params[1][0] == '#')
                    amount = to_uint32(trim(params[1].substr(1, params[1].length())));
                else
                {
                    tar_reg_idx = find(this->vir_regs_names, params[1]);
                    amount = (uint32_t)this->vir_regs[tar_reg_idx];
                }
            }
            tar_reg_idx = find(this->vir_regs_names, params[0]);
            if (params[2] == "LEFT")
                this->vir_regs[tar_reg_idx] = val << amount;
            else
                this->vir_regs[tar_reg_idx] = val >> amount;
        }
        else if (parts[0] == "cmp")
        {
            long int reg1_idx = find(this->vir_regs_names, params[0]);
            ;
            long int reg2_idx;
            if (params[1][0] == '#')
            {
                reg2_idx = to_autoNumber(params[1]);
                cmp = reg1_idx - reg2_idx;
            }
            else
            {
                reg2_idx = find(this->vir_regs_names, params[1]);
                cmp = this->vir_regs[reg1_idx] - this->vir_regs[reg2_idx];
            }
        }
        else if (parts[0] == "add")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] += reg2;
        }
        else if (parts[0] == "mul")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] *= reg2;
        }
        else if (parts[0] == "div")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] /= reg2;
        }
        else if (parts[0] == "sub")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] = sqrt(reg2);
        }
        else if (parts[0] == "pow")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] = pow(this->vir_regs[reg_idx], reg2);
        }
        else if (parts[0] == "incr")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            if (reg_idx > -1)
                this->vir_regs[reg_idx]++;
        }

        else if (parts[0] == "sub")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            long int reg2 = 0;

            if (params[1][0] == '#')
                reg2 = to_autoNumber(params[1].substr(1, params[1].length()));
            else
                reg2 = this->vir_regs[find(this->vir_regs_names, params[1])];

            if (reg_idx > -1)
                this->vir_regs[reg_idx] -= reg2;
        }

        else if (parts[0] == "decr")
        {
            const short int reg_idx = find(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx]--;
        }
        else if (parts[0] == "push")
        {
            if (params[0][0] == '#')
                this->vir_stack.push_back(to_autoNumber(trim(params[0])));
            else
            {
                const short int reg = find(this->vir_regs_names, params[0]);
                this->vir_stack.push_back(this->vir_regs[reg]);
            }
        }
        else if (parts[0] == "pull")
        {
            const uint16_t reg = find(this->vir_regs_names, params[0]);
            this->vir_regs[reg] = this->vir_stack[this->vir_stack.size() - 1];
            this->vir_stack = vector<logictype_t>(this->vir_stack.begin(), this->vir_stack.end() - 1);
        }

        else if (parts[0] == "exint")
        {
            _GXA_external_t target_external;
            for (uint32_t i = 0; i < this->externals.size(); i++)
            {
                if (this->externals[i].name == params[1])
                {
                    target_external = this->externals[i];
                    break;
                }
            }

            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = to_autoNumber(target_external.content);
        }
        else if (parts[0] == "argint")
        {
            _GXA_external_argument_t target_argument;

            for (uint32_t i = 0; i < this->external_arguments.size(); i++)
            {
                if (this->external_arguments[i].name == params[1])
                {
                    target_argument = this->external_arguments[i];
                    break;
                }
            }

            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = to_autoNumber(target_argument.value);
        }
        else if (parts[0] == "abs")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)abs(to_autoNumber(params[0]));
        }
        else if (parts[0] == "ceil")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)ceill(to_autoNumber(params[0]));
        }
        else if (parts[0] == "flr")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)floorl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "rnd")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)roundl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "sin")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)sinl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "cos")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)cosl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "sinh")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)sinhl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "ln")
        {
            const uint16_t reg_idx = find<string>(this->vir_regs_names, params[0]);
            this->vir_regs[reg_idx] = (logictype_t)logl(to_autoNumber(params[0]));
        }
        else if (parts[0] == "call")
        {
            _GXA_label_t target_label;
            for (uint16_t i = 0; i < this->labels.size(); i++)
            {
                if (this->labels[i].name == params[0])
                    target_label = this->labels[i];
            }

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

            const vector<_GXA_snippet_t> call_out = this->_transpile_(target_label.text, target_label.line_idx);
            output.insert(output.begin(), call_out.begin(), call_out.end());
        }
        else if (parts[0] == ".transpile")
        {
            _GXA_snippet_t snippet;
            for (string s : this->config.special_registers_name)
            {
                snippet.push_back(this->vir_regs[find<string>(this->vir_regs_names, s)]);
            }
            output.push_back(snippet);
        }
        else if (parts[0] == ".reset")
        {
            for (uint16_t i = 0; i < this->vir_regs.size(); i++)
            {
                this->vir_regs[i] = 0;
            }
        }
        else if (parts[0] == ".debug")
        {
            string text = lines[i].substr(lines[i].find('('), lines[i].find(')') - 1);
            cout << "[.debug, line " << (line_idx + i + 1) << "]: " << text << "\n";
        }
    }

    return output;
}

vector<_GXA_snippet_t> _GXA_::compile()
{
    const vector<string> lines = split(this->input, '\n');
    const vector<_GXA_label_t> labels = this->_slice_labels_(this->input);

    for (_GXA_label_t lbl : labels)
    {
        if (lbl.name == this->startLabel.name)
        {
            this->startLabel = lbl;
            break;
        }
    }

    this->_pre_processors_(this->input, vector<uint32_t>());
    this->_transpile_(this->startLabel.name, this->startLabel.line_idx);

    return this->output;
}