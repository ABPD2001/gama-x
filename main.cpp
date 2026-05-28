#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdint>
#include "./core/assembler.hpp"
#include "./core/linker.hpp"
#include "./core/lint.hpp"
#define VERSION "v2.0.0"
#define HELP "help..."

using std::cout;
using std::fstream;
using std::ios;
using std::uint64_t;

namespace fs = std::filesystem;

void print_error(string text);
void print_app_error(vector<_GXLT_error_t> errors, string format);
void parse_app_arguments(vector<string> argvv, vector<_GXL_file_t> &files, vector<string> &config_args, vector<string> &binary_logics, _GXA_config_t &assembler_config, _GXL_config_t &linker_config, string &error_format, string &output_filepath, bool &binary_output);
int main(int argc, char *argv[]);

void parse_app_arguments(vector<string> argvv, vector<_GXL_file_t> &files, vector<string> &config_args, vector<string> &binary_logics, _GXA_config_t &assembler_config, _GXL_config_t &linker_config, string &error_format, string &output_filepath, bool &binary_output)
{
    const string valid_valuar_flags[] = {"-c", "--config", "-m", "--metadata-filename", "-e", "--error-format", "-l", "--binary-logics", "-v", "--values", "-g", "--registers", "-r", "--repository", "-o", "--output"};
    const string valid_file_flags[] = {"-c", "--config", "-o", "--output", "-e", "--error-format"};

    for (uint64_t i = 0; i < argvv.size(); i++)
    {
        const string flag = argvv[i];

        if (flag == "-V" || flag == "--version")
        {
            cout << VERSION << "\n";
            exit(0);
        }
        else if (flag == "-h" || flag == "--help")
        {
            cout << HELP << "\n";
            exit(0);
        }
        else if (flag == "-i" || flag == "--ignore-repository")
        {
            linker_config.repository_check = false;
        }
        else if (flag == "-d" || flag == "--ignore-dependecies")
        {
            linker_config.dependecy_check = false;
        }
        else if (flag == "-b" || flag == "--binary")
        {
            binary_output = true;
        }

        else if (includes_arr<const string, 16>(valid_valuar_flags, flag))
        {
            if (i == argvv.size() - 1)
            {
                print_error("argument value '" + flag + "' missed!");
                exit(1);
            }
            i++;
            const string value = string(argvv[i]);
            if (includes_arr<const string, 6>(valid_file_flags, flag))
            {
                if (!fs::exists(value) && !(flag == "-o" || flag == "--output"))
                {
                    print_error("file path '" + value + "' for argument '" + flag + "' does not exists!");
                    exit(1);
                }
                if (flag == "-o" || flag == "--output")
                {
                    output_filepath = value;
                    continue;
                }

                fstream config_file;
                string content;
                string templ;

                config_file.open(value, ios::in);
                if (!config_file.is_open())
                {
                    print_error("failed to open '" + value + "' !");
                    exit(1);
                }

                while (getline(config_file, templ))
                {
                    content += templ + '\n';
                }
                if (content.length())
                    content = content.substr(0, content.length() - 1);
                if (flag == "-c" || flag == "--config")
                {
                    const vector<argument_t> config_arguments = parse_arguments(content);
                    for (uint32_t j = 0; j < config_arguments.size(); j++)
                    {
                        config_args.push_back(config_arguments[j].key);
                        config_args.push_back(config_arguments[j].value);
                    }
                }
                else if (flag == "-e" || flag == "--error-format")
                    error_format = content;
                else if (flag == "-m" || flag == "--metadata-filename")
                {
                    linker_config.metadata_file_name = value;
                }
            }
            else if (flag == "-r" || flag == "--repository")
            {
                if (!fs::is_directory(value))
                {
                    print_error("invalid path '" + value + "' as a repository!");
                    exit(1);
                }
                linker_config.repository_path = value;
            }
            else if (flag == "-v" || flag == "--values")
            {
                const vector<string> parts = split(value, ',');
                vector<logictype_t> values;
                for (string p : parts)
                {
                    values.push_back(to_autoNumber(p));
                }
                assembler_config.register_values = values;
            }
            else if (flag == "-g" || flag == "--registers")
            {
                const vector<string> parts = split(value, ',');
                assembler_config.registers_name = parts;
            }
            else if (flag == "-l" || flag == "--binary-logics")
            {
                const vector<string> logics = split(value, ',');
                const string valids[] = {"u64", "s64", "u32", "s32", "u16", "s16", "u8", "s8"};
                for (string l : logics)
                {
                    if (!includes_arr<const string, 8>(valids, l))
                    {
                        print_error("invalid argument '-b, --binary' value '" + value + "' !");
                        exit(1);
                    }
                }
                binary_logics = logics;
            }
            else if (flag == "-s" || flag == "--special-registers")
            {
                const vector<string> parts = split(value, ',');
                assembler_config.special_registers_name = parts;
            }
        }
        else
        {
            if (!fs::exists(flag))
            {
                print_error("invalid file path '" + flag + "' !");
                exit(1);
            }
            fstream temp_f;
            _GXL_file_t file;
            string templ;

            temp_f.open(flag, ios::in);
            if (!temp_f.is_open())
            {
                print_error("failed to open '" + flag + "' !");
                exit(1);
            }
            while (getline(temp_f, templ))
            {
                file.content += templ + '\n';
            }
            temp_f.close();
            file.name = fs::path(flag).filename().string();
            if (file.content.size())
                file.content = file.content.substr(0, file.content.length() - 1);
            files.push_back(file);
        }
    }
}

void print_error(string text)
{
    cout << "Gama-X: "
         << text << "\n";
}
void print_app_error(vector<_GXLT_error_t> errors, string format)
{
    if (!format.length())
        format = "Error in <FILE> at line <LINE>: <MESSAGE>\ntype <TYPE>, cause <CAUSE>";
    string temp = format;
    const string forms[] = {"FILE", "MESSAGE", "CAUSE", "TYPE", "LINE"};

    for (_GXLT_error_t err : errors)
    {
        const string vals[] = {err.file, err.message, err.cause, err.type, to_string(err.line)};
        for (uint32_t i = 0; i < 5; i++)
        {
            if (temp.find(forms[i]) == string::npos)
                continue;
            temp.replace(temp.find(forms[i]), forms[i].length(), vals[i]);
        }

        cout << "Gama-X: " << temp << "\n";
        temp = format;
    }
}

int main(int argc, char *argv[])
{

    vector<_GXL_file_t> files;
    vector<string> config_args;
    vector<string> binary_logics;
    vector<string> argvv;
    _GXA_config_t assembler_config = {vector<string>(), vector<string>(), vector<logictype_t>()};
    _GXL_config_t linker_config = {"", "metadata.riff", true, true};

    string error_format;
    string output_filepath;
    bool binary_output = false;

    for (uint32_t i = 1; i < argc; i++)
    {
        argvv.push_back(string(argv[i]));
    }

    parse_app_arguments(argvv, files, config_args, binary_logics, assembler_config, linker_config, error_format, output_filepath, binary_output);
    parse_app_arguments(config_args, files, config_args, binary_logics, assembler_config, linker_config, error_format, output_filepath, binary_output);

    // first link files.
    _GXA_ _assembler_;
    _GXL_ _linker_;
    _GXLT_ _lint_;

    _linker_.init(files, linker_config);
    _linker_.merge();
    if (_linker_.errors.size())
    {
        cout << "<--- Gama-X Linker --->" << "\n";
        print_app_error(_linker_.errors, error_format);
        exit(1);
    }

    // then, just check errors (linting).

    for (_GXL_file_t f : files)
    {
        _assembler_.init(f.content, assembler_config);

        vector<_GXA_external_t> ext_temp;
        vector<_GXA_external_argument_t> ext_arg_temp;
        vector<_GXA_label_t> labels = _assembler_._slice_labels_(f.content);
        vector<_GXLT_error_t> prep_errors;
        vector<uint32_t> prep_ignlines;
        string content_cpy = f.content;

        _lint_.init(ext_temp, ext_arg_temp, labels, assembler_config.registers_name, assembler_config.register_values);
        prep_errors = _lint_.check_pre_processors_errors(f.name, f.content);

        for (_GXLT_error_t err : prep_errors)
        {
            prep_ignlines.push_back(err.line);
        }

        _assembler_._pre_processors_(content_cpy, prep_ignlines);
        for (_GXA_label_t lbl : labels)
        {
            vector<_GXLT_error_t> errs = _lint_.check_errors(f.name, lbl.text, lbl.line_idx);
        }
    }
    if (_lint_.total_errors.size())
    {
        print_app_error(_lint_.total_errors, error_format);
        exit(1);
    }

    // at the end, if everything was fine, create output.

    _assembler_.init(_linker_.merged, assembler_config);
    vector<_GXA_snippet_t> snippets = _assembler_.compile();
    string output;

    for (_GXA_snippet_t sn : snippets)
    {
        for (uint32_t i = 0; i < sn.size(); i++)
        {
            if (binary_output)
            {
                if (binary_logics.size() != sn.size())
                {
                    print_error("binary logics count does not match with the output snippet size!");
                    exit(1);
                }

                const string modes_name[] = {"s64", "u32", "s32", "u16", "s16", "u8", "s8"};
                if (binary_logics[i] != "u64")
                {
                    output += toBinary(sn[i], find_arr<const string, 7>(modes_name, binary_logics[i]));
                }
                else
                    output += toBinaryu64(sn[i]);
            }
            else
            {
                output += to_string(sn[i]) + ',';
            }
            if (output.size())
                output = output.substr(0, output.length() - 1);
        }
    }
    if (output.size())
    {
        fstream output_f;

        if (fs::exists(output_filepath))
            output_f.open(output_filepath, ios::out | ios::trunc);
        else
            output_f.open(output_filepath, ios::out);

        output_f << output;
        output_f.close();
    }
    else
    {
        print_error("there is nothing to output!");
        exit(1);
    }

    return 0;
}