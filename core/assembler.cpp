#include "./assembler.hpp"

_GXA_::_GXA_(string content, _GXA_config_t config = {})
{
    this->input = content;
    this->config = config;
}
_GXA_::_GXA_() {}
_GXA_::~_GXA_()
{
    this->status = 3;
    this->errors = vector<_GXA_error_t>();
    this->labels = vector<_GXA_label_t>();
    this->input.clear();
    this->startLabel.clear();
    this->vir_regs_names = vector<string>();
    this->vir_regs = vector<logictype_t>();
}

vector<_GXA_external_argument_t> _GXA_::_parse_argular_(string path, uint32_t index)
{
    fstream extern_f;
    vector<string> lines;
    string line;

    extern_f.open(path, ios::in);

    if (!extern_f.is_open())
    {
        _GXA_error_t error = {"Failed to open file in '" + path + "'!", "FILE_READ_FAILURE", "input-output", index};
        this->errors.push_back(error);
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
                    _GXA_error_t error = {"Invalid syntax for" + name + "arguments value!", "INVALID_ARGUMNET_VALUE", "input-output", index};
                    this->errors.push_back(error);
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
        _GXA_error_t error = {"Failed to open file in '" + path + "'!", "FILE_READ_FAILURE", "input-output", index};
        this->errors.push_back(error);
        return;
    }
    while (getline(extern_f, line))
    {
        content += line + '\n';
    }

    content.resize(content.length() + 1);
    content[content.length() - 1] = '\0';

    _GXA_external_t external = {content, name};
    this->externals.push_back(external);
}

void _GXA_::_check_pre_processors_errors_()
{
    const vector<string> lines = split(this->input, '\n');

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        string tmp_line = trim(lines[i]);
        if (lines[i][0] == '.')
        {
            tmp_line = tmp_line.substr(1);
            vector<string> parts = split(tmp_line, ' ');
            _GXA_error_t error;

            if (tmp_line == "replace" && (parts.size() != 3 || parts[1].empty() || parts[2].empty()))
            {

                error = {"'.replace' pre-processor set arguments missed!", "CO_PROCCESSOR_VALUES_MISSED", "syntax", i};
                this->errors.push_back(error);
            }
            else if (tmp_line == "main" && (parts.size() != 2 || parts[1].empty()))
            {
                error = {"'.main' pre-proccessor set argument missed!", "CO_PROCCESSOR_VALUE_MISSED", "syntax", i};
                this->errors.push_back(error);
            }
            else if ((tmp_line == "argular" || tmp_line == "extern") && (parts[1].find_first_of('"') == parts[1].find_last_of('"') || parts[1].find_first_of('"') == string::npos))
            {
                error = {string("'.") + parts[0] + "' pre-proccessor set invalid argument syntax!", "CO_PROCCESSOR_INVALID_ARGUMENT_SYNTAX", "syntax", i};
                this->errors.push_back(error);
            }
            else if ((tmp_line == "argular" || tmp_line == "extern") && (parts.size() != 2 || !fs::exists(parts[1])))
            {
                error = {"'.argular' pre-proccessor set argument isnt exists as a file!", "CO_PROCCESSOR_INVALID_FILE_PATH", "syntax", i};
                this->errors.push_back(error);
            }
            else if (tmp_line == ".end" && parts.size() != 1)
            {
                error = {"'.end' pre-proccessor set does not accepts any arguments!", "CO_PROCCESSOR_INVALID_ARGUMENTS", "syntax", i};
                this->errors.push_back(error);
            }
            else
            {
                error = {string("'.") + parts[0] + "' pre-proccessor set isnt valid!", "CO_PROCCESSOR_INVALID_INSTRUCTION", "syntax", i};
                this->errors.push_back(error);
            }
        }
    }
}

void _GXA_::_check_errors_(string content, uint32_t index)
{
    const string valid_instructions[27] = {"mov", "add", "sub", "mul", "div", "abs", "ln", "cos", "tan", "sin", "sinh", "ceil", "flr", "rnd", "call", "cmp", "push", "pull", "logi", "shif", "incr", "decr", "exint", "argint", ".transpile", ".debug", ".reset"};
    const string three_register_instructions[8] = {"add", "sub", "mul", "div", "shif", "logi", "argint", "exint"};
    const string mathmatic_instructions[4] = {"add", "sub", "mul", "div"};
    const string mathmatic_two_reg[10] = {"abs", "ln", "cos", "tan", "sin", "sinh", "ceil", "flr", "rnd", "cmp"};
    const string one_argument_instructions[5] = {"pull", "push", "incr", "decr", "call"};
    const string one_register_arg_instructions[4] = {"pull", "push", "incr", "decr"};
    const string two_one_op_reg_instructions[2] = {"shif", "logi"};
    const string one_reg_one_name_instructions[2] = {"exint", "argint"};
    const string no_arg_instructions[2] = {".reset", ".transpile"};

    vector<string> lines = split(content, '\n');

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        const string line = trim(lines[i]);
        const vector<string> parts = split(line, ' ');
        const vector<string> args = split(parts[1], ',');
        _GXA_error_t error;

        if (!includes_arr<const string>(valid_instructions, parts[0]))
        {
            error = {"invalid instruction '" + parts[0] + string("'!"), "INVALID_INSTRUCTION", "syntax", index + i};
            this->errors.push_back(error);
        }
        else if (includes_arr<const string>(mathmatic_two_reg, parts[0]))
        {
            if (args.size() != 2)
            {
                error = {(args.size() < 2 ? "to few" : "to much") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (args[0][0] == '#')
            {
                error = {"destination argument (first argument) cannot be a literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (args[1][0] == '#' && !isValidNumber(args[1].substr(1)))
            {
                error = {"second argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                this->errors.push_back(error);
            }
            else if (!includes<string>(this->vir_regs_names, args[1]))
            {
                error = {"invalid register refrenced '" + args[1] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                this->errors.push_back(error);
            }
        }

        else if (includes_arr<const string>(three_register_instructions, parts[0]))
        {
            if (args.size() != 3)
            {
                error = {(args.size() < 3 ? "to few" : "to much") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                this->errors.push_back(error);
            }
        }
        else if (includes_arr<const string>(mathmatic_instructions, parts[0]))
        {
            if (args[0][0] == '#')
            {
                error = {"first argument cannot be a literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (args[1][0] == '#' && !isValidNumber(args[1].substr(1)))
            {
                error = {"second argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (args[2][0] == '#' && !isValidNumber(args[1].substr(1)))
            {
                error = {"third argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                this->errors.push_back(error);
            }
            for (uint8_t j = 0; j < 3; j++)
            {
                if (args[j][0] != '#' && !includes<string>(this->vir_regs_names, args[j]))
                {
                    error = {"invalid register refrenced '" + args[j] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                    this->errors.push_back(error);
                }
            }
        }
        else if (includes_arr<const string>(one_argument_instructions, parts[0]))
        {
            if (args.size() != 1)
            {
                error = {(args.size() > 1 ? "to much" : "to few") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                this->errors.push_back(error);
            }
        }
        else if (includes_arr<const string>(one_register_arg_instructions, parts[0]))
        {
            if (!includes(this->vir_regs_names, args[0]))
            {
                error = {"invalid register refrenced '" + args[0] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                this->errors.push_back(error);
            }
        }
        else if (includes_arr<const string>(no_arg_instructions, parts[0]))
        {
            if (!args[0].empty())
            {
                error = {parts[0] + " instruction does not accept any arguments!", toUppercase(parts[0].substr(1)) + "_INVALID_SYNTAX", "syntax", index + i};
                this->errors.push_back(error);
            }
        }
        else if (includes_arr<const string>(one_reg_one_name_instructions, parts[0]))
        {
            if (args[0][0] == '#')
            {
                error = {"first argument cannot be literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (!includes(this->vir_regs_names, args[0]))
            {
                error = {"invalid register refrenced '" + args[0] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                this->errors.push_back(error);
            }
            if (parts[0] == "argint")
            {
                bool found = false;

                for (_GXA_external_argument_t arg : this->external_arguments)
                {
                    if (arg.name == args[2])
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    error = {"invalid external argument refrenced '" + args[2] + "'!", "INVALID_EXTERNAL_ARGUMENT_REFRENCED", "syntax", index + i};
                    this->errors.push_back(error);
                }
            }
            else if (parts[0] == "exint")
            {
                bool found = false;

                for (_GXA_external_t arg : this->externals)
                {
                    if (arg.name == args[2])
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    error = {"invalid external refrenced '" + args[2] + "'!", "INVALID_EXTERNAL_REFRENCED", "syntax", index + i};
                    this->errors.push_back(error);
                }
            }
        }
        else if (parts[0] == "call")
        {
            const string valid_conditions[6] = {"LE", "LS", "EQ", "NE", "GE", "GT"};
            bool found = false;

            if (args.size() < 1 || args.size() > 2)
            {
                error = {(args.size() > 2 ? "to much" : "to few") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                this->errors.push_back(error);
            }

            for (uint32_t j = 0; j < this->labels.size(); j++)
            {
                if (this->labels[j].name == args[0])
                    found = true;
            }

            if (!found)
            {
                error = {"invalid label '" + args[0] + "' refrenced!", "INVALID_LABEL_REFRENCED", "syntax", index + i};
                this->errors.push_back(error);
            }

            if (args.size() == 2 && !includes_arr<const string>(valid_conditions, args[1]))
            {
                error = {"invalid condition '" + args[1] + "' refrenced!", "INVALID_CONDITION_REFRENCED", "syntax", index + i};
                this->errors.push_back(error);
            }
        }
        else if (parts[0] == ".debug" && (lines[i].find('(') == string::npos || lines[i].find(')') == string::npos))
        {
            error = {"invalid syntax for .debug instruction!", "INVALID_DEBUG_SYNTAX", "syntax", index + i};
            this->errors.push_back(error);
            continue;
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
                    _GXA_error_t error = {"label " + cur_label + " overloaded!", "LABEL_OVERLOADED", "syntax", i};
                    this->errors.push_back(error);
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

void _GXA_::_pre_processors_(string &content)
{
    vector<string> lines = split(content, '\n');
    char empties[7] = {'\r', '\n', ' ', '\a', '\b', '\t', '\f'};

    for (uint32_t i = 0; i < lines.size(); i++)
    {
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
            this->startLabel = parts[1];
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

        for (uint8_t i = 0; i < params.size(); i++)
        {
            if (filter(params[i], empties, 7).empty())
            {
                _GXA_error_t error = {"'" + parts[0] + "' instruction set arguemnt " + to_string(i + 1) + " has invalid syntax!", "INVALID_VALUE_SYNTAX", "syntax", lx};
                this->errors.push_back(error);
            }
        }

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
        }
        else if (parts[0] == ".transpile")
        {
            _GXA_snippet_t snippet = {(uint64_t)this->vir_regs[2], (uint32_t)(this->vir_regs[0]), (uint32_t)(this->vir_regs[1])};
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