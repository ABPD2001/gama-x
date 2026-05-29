#include "./lint.hpp"

_GXLT_::_GXLT_() {};
_GXLT_::_GXLT_(vector<_GXA_external_t> &external, vector<_GXA_external_argument_t> &external_arguments, vector<_GXA_label_t> &labels, vector<string> &vir_regs_name, vector<logictype_t> &vir_regs)
{
    this->externals = external;
    this->external_arguments = external_arguments;
    this->labels = labels;
    this->vir_regs_names = vir_regs_name;
    this->vir_regs = vir_regs;
}

void _GXLT_::init(vector<_GXA_external_t> &external, vector<_GXA_external_argument_t> &external_arguments, vector<_GXA_label_t> &labels, vector<string> &vir_regs_name, vector<logictype_t> &vir_regs)
{
    this->externals = external;
    this->external_arguments = external_arguments;
    this->labels = labels;
    this->vir_regs_names = vir_regs_name;
    this->vir_regs = vir_regs;
}

vector<_GXLT_error_t> _GXLT_::check_label_errors(string filename)
{
    vector<_GXLT_error_t> errors;
    vector<string> lbl_names_cpy;
    _GXLT_error_t error;

    for (_GXA_label_t lbl : this->labels)
    {
        if (!includes<string>(lbl_names_cpy, lbl.name))
            lbl_names_cpy.push_back(lbl.name);
        else
        {
            error = {filename, "label " + lbl.name + " overloaded!", "LABEL_OVERLOADED", "syntax", lbl.line_idx};
            errors.push_back(error);
            continue;
        }
    }

    this->total_errors.insert(this->total_errors.begin(), errors.begin(), errors.end());
    return errors;
};

vector<_GXLT_error_t> _GXLT_::check_pre_processors_errors(string filename, string input)
{
    vector<_GXLT_error_t> errors;
    const vector<string> lines = split(input, '\n');
    const string valid_preprocessors[7] = {".end", ".replace", ".import", ".extern", ".argular", ".main", ".include"};

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        string tmp_line = trim(lines[i]);
        if (lines[i][0] == '.')
        {
            vector<string> parts = split(tmp_line, ' ');
            _GXLT_error_t error;

            if (tmp_line == ".replace" && (parts.size() != 3 || parts[1].empty() || parts[2].empty()))
            {

                error = {filename, "'.replace' pre-processor set arguments missed!", "CO_PROCCESSOR_VALUES_MISSED", "syntax", i};
                errors.push_back(error);
            }
            else if (tmp_line == ".main" && (parts.size() != 2 || parts[1].empty()))
            {
                error = {filename, "'.main' pre-proccessor set argument missed!", "CO_PROCCESSOR_VALUE_MISSED", "syntax", i};
                errors.push_back(error);
            }
            else if ((tmp_line == ".argular" || tmp_line == ".extern") && (parts[1].find_first_of('"') == parts[1].find_last_of('"') || parts[1].find_first_of('"') == string::npos))
            {
                error = {filename, string("'.") + parts[0] + "' pre-proccessor set invalid argument syntax!", "CO_PROCCESSOR_INVALID_ARGUMENT_SYNTAX", "syntax", i};
                errors.push_back(error);
            }
            else if ((tmp_line == ".argular" || tmp_line == ".extern") && (parts.size() != 2 || !fs::exists(parts[1])))
            {
                error = {filename, "'.argular' pre-proccessor set argument isnt exists as a file!", "CO_PROCCESSOR_INVALID_FILE_PATH", "syntax", i};
                errors.push_back(error);
            }
            else if (tmp_line == ".end" && parts.size() != 1)
            {
                error = {filename, "'.end' pre-proccessor set does not accepts any arguments!", "CO_PROCCESSOR_INVALID_ARGUMENTS", "syntax", i};
                errors.push_back(error);
            }
            else if (!includes_arr<const string, 7>(valid_preprocessors, parts[0]))
            {
                error = {filename, string("'") + parts[0] + "' pre-proccessor set isnt valid!", "CO_PROCCESSOR_INVALID_INSTRUCTION", "syntax", i};
                errors.push_back(error);
            }
        }
    }

    this->total_errors.insert(this->total_errors.begin(), errors.begin(), errors.end());
    return errors;
}

vector<_GXLT_error_t> _GXLT_::check_errors(string filename, string content, uint32_t index)
{
    vector<_GXLT_error_t> errors;
    const string valid_instructions[] = {"mov", "add", "sub", "mul", "div", "abs", "ln", "cos", "tan", "sin", "sinh", "ceil", "flr", "rnd", "call", "cmp", "push", "pull", "logi", "shif", "incr", "decr", "exint", "argint", ".transpile", ".debug", ".reset"};
    const string three_register_instructions[] = {"add", "sub", "mul", "div", "shif", "logi", "argint", "exint"};
    const string mathmatic_instructions[] = {"add", "sub", "mul", "div"};
    const string mathmatic_two_reg[] = {"abs", "ln", "cos", "tan", "sin", "sinh", "ceil", "flr", "rnd", "cmp"};
    const string one_argument_instructions[] = {"pull", "push", "incr", "decr", "call"};
    const string one_register_arg_instructions[] = {"pull", "push", "incr", "decr"};
    const string two_one_op_reg_instructions[] = {"shif", "logi"};
    const string one_reg_one_name_instructions[] = {"exint", "argint"};
    const string no_arg_instructions[] = {".reset", ".transpile"};

    vector<string> lines = split(content, '\n');

    for (uint32_t i = 0; i < lines.size(); i++)
    {
        const string line = trim(lines[i]);
        const vector<string> parts = split(line, ' ');
        vector<string> args;
        char empties[7] = {'\r', '\n', ' ', '\a', '\b', '\t', '\f'};

        if (filter(line, empties, 7).empty() || parts[0].empty())
            continue;

        if (parts.size() > 1)
            args = split(parts[1], ',');

        _GXLT_error_t error;

        for (uint8_t i = 0; i < args.size(); i++)
        {
            if (filter(args[i], empties, 7).empty())
            {
                _GXLT_error_t error = {filename, "'" + parts[0] + "' instruction set arguemnt " + to_string(i + 1) + " has invalid syntax!", "INVALID_VALUE_SYNTAX", "syntax", i + index};
                errors.push_back(error);
            }
        }

        if (!includes_arr<const string, 27>(valid_instructions, parts[0]))
        {
            error = {filename, "invalid instruction '" + parts[0] + string("'!"), "INVALID_INSTRUCTION", "syntax", index + i};
            errors.push_back(error);
        }
        else if (includes_arr<const string, 10>(mathmatic_two_reg, parts[0]))
        {
            if (args.size() != 2)
            {
                error = {filename, (args.size() < 2 ? "to few" : "to much") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                errors.push_back(error);
            }
            if (args.size() >= 1 && args[0][0] == '#')
            {
                error = {filename, "destination argument (first argument) cannot be a literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                errors.push_back(error);
            }
            if (args.size() >= 2 && args[1][0] == '#' && !isValidNumber(args[1].substr(1)))
            {
                error = {filename, "second argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                errors.push_back(error);
            }
            else if (args.size() >= 2 && !includes<string>(this->vir_regs_names, args[1]))
            {
                error = {filename, "invalid register refrenced '" + args[1] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                errors.push_back(error);
            }
        }

        else if (includes_arr<const string, 8>(three_register_instructions, parts[0]))
        {
            if (args.size() != 3)
            {
                error = {filename, (args.size() < 3 ? "to few" : "to much") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                errors.push_back(error);
            }
        }
        else if (includes_arr<const string, 4>(mathmatic_instructions, parts[0]))
        {
            if (args.size() >= 1 && args[0][0] == '#')
            {
                error = {filename, "first argument cannot be a literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                errors.push_back(error);
            }
            if (args.size() >= 2 && args[1][0] == '#' && !isValidNumber(args[1].substr(1)))
            {
                error = {filename, "second argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                errors.push_back(error);
            }
            if (args.size() >= 3 && args[2][0] == '#' && !isValidNumber(args[2].substr(1)))
            {
                error = {filename, "third argumnet isnt a valid number!", "INVALID_ARGUMENT_SYNTAX", "syntax", index + i};
                errors.push_back(error);
            }
            for (uint8_t j = 0; j < 3; j++)
            {
                if (args[j][0] != '#' && !includes<string>(this->vir_regs_names, args[j]))
                {
                    error = {filename, "invalid register refrenced '" + args[j] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                    errors.push_back(error);
                }
            }
        }
        else if (includes_arr<const string, 5>(one_argument_instructions, parts[0]))
        {
            if (args.size() != 1)
            {
                error = {filename, (args.size() > 1 ? "to much" : "to few") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                errors.push_back(error);
            }
        }
        else if (includes_arr<const string, 4>(one_register_arg_instructions, parts[0]))
        {
            if (!includes(this->vir_regs_names, args[0]))
            {
                error = {filename, "invalid register refrenced '" + args[0] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                errors.push_back(error);
            }
        }
        else if (includes_arr<const string, 2>(no_arg_instructions, parts[0]))
        {
            if (args.size() >= 1 && !args[0].empty())
            {
                error = {filename, parts[0] + " instruction does not accept any arguments!", toUppercase(parts[0].substr(1)) + "_INVALID_SYNTAX", "syntax", index + i};
                errors.push_back(error);
            }
        }
        else if (includes_arr<const string, 2>(one_reg_one_name_instructions, parts[0]))
        {
            if (args.size() >= 1 && args[0][0] == '#')
            {
                error = {filename, "first argument cannot be literal value!", "FIRST_ARGUMENT_LITERAL", "syntax", index + i};
                errors.push_back(error);
            }
            if (args.size() >= 1 && !includes(this->vir_regs_names, args[0]))
            {
                error = {filename, "invalid register refrenced '" + args[0] + "'!", "INVALID_REGISTER_REFRENCED", "syntax", index + i};
                errors.push_back(error);
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
                    error = {filename, "invalid external argument refrenced '" + args[2] + "'!", "INVALID_EXTERNAL_ARGUMENT_REFRENCED", "syntax", index + i};
                    errors.push_back(error);
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
                    error = {filename, "invalid external refrenced '" + args[2] + "'!", "INVALID_EXTERNAL_REFRENCED", "syntax", index + i};
                    errors.push_back(error);
                }
            }
        }
        else if (parts[0] == "call")
        {
            const string valid_conditions[] = {"LE", "LS", "EQ", "NE", "GE", "GT"};
            bool found = false;

            if (args.size() < 1 || args.size() > 2)
            {
                error = {filename, (args.size() > 2 ? "to much" : "to few") + string(" arguments for '") + parts[0] + string("' instruction!"), args.size() < 3 ? "FEW_ARGUMENTS" : "MUCH_ARGUMENTS", "syntax", index + i};
                errors.push_back(error);
            }

            for (uint32_t j = 0; j < this->labels.size(); j++)
            {
                if (this->labels[j].name == args[0])
                    found = true;
            }

            if (!found)
            {
                error = {filename, "invalid label '" + args[0] + "' refrenced!", "INVALID_LABEL_REFRENCED", "syntax", index + i};
                errors.push_back(error);
            }

            if (args.size() == 2 && !includes_arr<const string, 6>(valid_conditions, args[1]))
            {
                error = {filename, "invalid condition '" + args[1] + "' refrenced!", "INVALID_CONDITION_REFRENCED", "syntax", index + i};
                errors.push_back(error);
            }
        }
        else if (parts[0] == ".debug" && (lines[i].find('(') == string::npos || lines[i].find(')') == string::npos))
        {
            error = {filename, "invalid syntax for .debug instruction!", "INVALID_DEBUG_SYNTAX", "syntax", index + i};
            errors.push_back(error);
            continue;
        }
    }

    this->total_errors.insert(this->total_errors.begin(), errors.begin(), errors.end());
    return errors;
}