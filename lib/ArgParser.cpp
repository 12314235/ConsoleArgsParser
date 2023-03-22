#include "ArgParser.h"
#include <iostream>
#include <string_view>

ArgumentParser::ArgParser::ArgParser(const std::string& name)
{
	parser_name = name;
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& argv)
{
	ArgumentParser::ArgParser::args_string* buff_str;
	ArgumentParser::ArgParser::args_int* buff_int;
	ArgumentParser::ArgParser::args_bool* buff_bool;
	ArgumentParser::ArgParser::help* help;

	bool is_parsed = true;

	std::string_view param;
	std::string value;

	for (int i = 1; i < argv.size(); i++) {
		if (argv[i][0] == '-' && argv[i][1] != '-') {
			if (argv[i].size() > 2 && argv[i][2] == '=') {
				param = { std::to_string(argv[i][1]) };
				value = argv[i].substr(argv[i].find_first_of('=') + 1, argv[i].size() - argv[i].find_first_of('=') - 1);
				for (int j = 0; j < Values_Storage.size(); j++) {
					if (std::holds_alternative<args_string>(Values_Storage[j])) {
						buff_str = std::get_if<ArgumentParser::ArgParser::args_string>(&Values_Storage[j]);
						if (buff_str->short_param.compare(param)) {
							buff_str->string_value = value;
							buff_str->is_used = true;
						}
					}
					else if (std::holds_alternative<args_int>(Values_Storage[j])) {
						buff_int = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[j]);
						if (buff_int->param.compare(param)) {
							if (buff_int->is_multi_value) {
								buff_int->int_values.push_back(std::stoll(value));
							}
						}
					}
					else if (std::holds_alternative<args_bool>(Values_Storage[j])) {
						buff_bool = std::get_if<args_bool>(&Values_Storage[j]);
						if (buff_bool->param == argv[i]) {
							if (buff_bool->bool_value_ptr != nullptr) {
								*buff_bool->bool_value_ptr = true;
							}
							else {
								buff_bool->bool_value = true;
							}
							buff_bool->is_used = true;
						}
					}
				}
			}
			else {
				for (int k = 1; k < argv[i].size(); k++) {
					for (int j = 0; j < Values_Storage.size(); j++) {
						if (std::holds_alternative<args_string>(Values_Storage[j])) {
							buff_str = std::get_if<ArgumentParser::ArgParser::args_string>(&Values_Storage[j]);
							if (buff_str->short_param == std::to_string(argv[i][k])) {
								if (buff_str->string_value_ptr != nullptr) {
									*buff_str->string_value_ptr = value;
								}
								else {
									buff_str->string_value = value;
								}
								buff_str->is_used = true;
							}
						}
						else if (std::holds_alternative<args_int>(Values_Storage[j])) {
							buff_int = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[j]);
							if (buff_int->short_param == std::to_string(argv[i][k])) {
								if (buff_int->int_value_ptr != nullptr) {
									*buff_int->int_value_ptr = std::stoll(value);
								}
								else {
									buff_int->int_value = std::stoll(value);
								}
								buff_int->is_used = true;
							}
						}
						else if (std::holds_alternative<args_bool>(Values_Storage[j])) {
							buff_bool = std::get_if<args_bool>(&Values_Storage[j]);
							if (buff_bool->short_param == argv[i][k]) {
								if (buff_bool->bool_value_ptr != nullptr) {
									*buff_bool->bool_value_ptr = true;
								}
								else {
									buff_bool->bool_value = true;
								}
								buff_bool->is_used = true;
							}
						}
						else if (std::holds_alternative<ArgumentParser::ArgParser::help>(Values_Storage[j])) {
							help = std::get_if<ArgumentParser::ArgParser::help>(&Values_Storage[j]);
							if (help->short_param == argv[i][k]) {
								std::cout << help->information;
							}
						}
					}
				}
			}
		}
		else if (argv[i][0] == '-' && argv[i][1] == '-') {
			param = { argv[i].substr(2, argv[i].find_first_of('=') - 2) };
			value = argv[i].substr(argv[i].find_first_of('=') + 1, argv[i].size() - argv[i].find_first_of('=') - 1);
			for (int j = 0; j < Values_Storage.size(); j++) {
				if (std::holds_alternative<args_string>(Values_Storage[j])) {
					buff_str = std::get_if<ArgumentParser::ArgParser::args_string>(&Values_Storage[j]);
					if (buff_str->param.compare(param)) {
						if (buff_str->string_value_ptr != nullptr) {
							*buff_str->string_value_ptr = value;
						}
						else {
							buff_str->string_value = value;
						}
						buff_str->is_used = true;
					}
				}
				else if (std::holds_alternative<args_int>(Values_Storage[j])) {
					buff_int = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[j]);
					if (buff_int->param.compare(param)) {
						if (buff_int->is_multi_value) {
							if (buff_int->int_values_ptr != nullptr) {
								buff_int->int_values_ptr->push_back(std::stoll(value));
								buff_int->is_used = buff_int->int_values_ptr->size() >= buff_int->min_args_count;
							}
							else {
								buff_int->int_values.push_back(std::stoll(value));
								buff_int->is_used = true && (buff_int->int_values.size() >= buff_int->min_args_count);
							}
						}
						else {
							buff_int->int_value = std::stoll(value);
							buff_int->is_used = true;
						}
					}
				}
				else if (std::holds_alternative<args_bool>(Values_Storage[j])) {
					buff_bool = std::get_if<args_bool>(&Values_Storage[j]);
					if (buff_bool->param.compare(param)) {
						if (buff_bool->bool_value_ptr != nullptr) {
							*buff_bool->bool_value_ptr = true;
						}
						else {
							buff_bool->bool_value = true;
						}
						buff_bool->is_used = true;
					}

				}
				else if (std::holds_alternative<ArgumentParser::ArgParser::help>(Values_Storage[j])) {
					help = std::get_if<ArgumentParser::ArgParser::help>(&Values_Storage[j]);
					if (help->param.compare(param)) {
						std::cout << help->information;
					}
				}
			}
		}
		else {
			for (int j = 0; j < Values_Storage.size(); j++) {
				if (std::holds_alternative<args_int>(Values_Storage[j])) {
					buff_int = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[j]);
					if (buff_int->is_positional) {
						if (buff_int->is_multi_value) {
							if (buff_int->int_values_ptr != nullptr) {
								buff_int->int_values_ptr->push_back(std::stoll(argv[i]));
								buff_int->is_used = buff_int->int_values_ptr->size() >= buff_int->min_args_count;
							}
							else {
								buff_int->int_values.push_back(std::stoll(argv[i]));
								buff_int->is_used = true && (buff_int->int_values.size() >= buff_int->min_args_count);
							}
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<args_string>(Values_Storage[i])) {
			buff_str = std::get_if<ArgumentParser::ArgParser::args_string>(&Values_Storage[i]);
			if (!buff_str->is_used) {
				is_parsed = false;
				break;
			}
		}
		else if (std::holds_alternative<args_int>(Values_Storage[i])) {
			buff_int = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[i]);
			if (!buff_int->is_used) {
				is_parsed = false;
				break;
			}
		}
	}

	return is_parsed;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv)
{
	static std::vector<std::string> buff;

	for (int i = 0; i < argc; i++) {
		buff.push_back(argv[i]);
	}

	return Parse(buff);
}

ArgumentParser::ArgParser::args_string& ArgumentParser::ArgParser::AddStringArgument(const std::string& param)
{
	args_string new_argument;

	new_argument.param = param;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_string&>(Values_Storage[Values_Storage.size() - 1]);;
}

ArgumentParser::ArgParser::args_string& ArgumentParser::ArgParser::AddStringArgument(char short_param, const std::string& param)
{
	args_string new_argument;

	new_argument.param = param;
	new_argument.short_param = short_param;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_string&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_string& ArgumentParser::ArgParser::AddStringArgument(char short_param, const std::string& param, const std::string& information)
{
	args_string new_argument;

	new_argument.param = param;
	new_argument.short_param = short_param;
	new_argument.information = information;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_string&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_int& ArgumentParser::ArgParser::AddIntArgument(const std::string& param)
{
	args_int new_argument;

	new_argument.param = param;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_int&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_int& ArgumentParser::ArgParser::AddIntArgument(char short_param, const std::string& param)
{
	args_int new_argument;

	new_argument.param = param;
	new_argument.short_param = short_param;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_int&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_int& ArgumentParser::ArgParser::AddIntArgument(const std::string& param, const std::string& information)
{
	args_int new_argument;

	new_argument.param = param;
	new_argument.information = information;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_int&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_bool& ArgumentParser::ArgParser::AddFlag(char flag, const std::string& name)
{
	args_bool new_argument;

	new_argument.short_param = flag;
	new_argument.param = name;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_bool&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_bool& ArgumentParser::ArgParser::AddFlag(char flag, const std::string& name, const std::string& information)
{
	args_bool new_argument;

	new_argument.short_param = flag;
	new_argument.param = name;
	new_argument.information = information;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_bool&>(Values_Storage[Values_Storage.size() - 1]);
}

ArgumentParser::ArgParser::args_bool& ArgumentParser::ArgParser::AddFlag(const std::string& name, const std::string& information)
{
	args_bool new_argument;

	new_argument.param = name;
	new_argument.information = information;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::args_bool&>(Values_Storage[Values_Storage.size() - 1]);
}

const std::string& ArgumentParser::ArgParser::GetStringValue(const std::string& param)
{
	ArgumentParser::ArgParser::args_string* buff;
	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<args_string>(Values_Storage[i])) {
			buff = std::get_if<ArgumentParser::ArgParser::args_string>(&Values_Storage[i]);
			if (buff->param == param) {
				return buff->string_value;
			}
		}
	}
	return "";
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& param)
{
	ArgumentParser::ArgParser::args_int* buff;
	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<args_int>(Values_Storage[i])) {
			buff = std::get_if<ArgumentParser::ArgParser::args_int>(&Values_Storage[i]);
			if (buff->param == param) {
				return buff->int_value;
			}
		}
	}
	return 0;
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& param, int index)
{
	ArgumentParser::ArgParser::args_int* buff;
	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<args_int>(Values_Storage[i])) {
			buff = std::get_if<args_int>(&Values_Storage[i]);
			if (buff->param == param) {
				if (!buff->is_multi_value) {
					throw;
				}
				if (buff->int_values_ptr != nullptr) {
					buff->int_values = *buff->int_values_ptr;
				}
				return buff->int_values[index];
			}
		}
	}
	return 0;
}

bool ArgumentParser::ArgParser::GetFlag(const std::string name)
{
	ArgumentParser::ArgParser::args_bool* buff;
	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<args_bool>(Values_Storage[i])) {
			buff = std::get_if<args_bool>(&Values_Storage[i]);
			if (buff->param == name) {
				return buff->bool_value;
			}
		}
	}
	return false;
}

ArgumentParser::ArgParser::help& ArgumentParser::ArgParser::AddHelp(char short_operation, const std::string& operation, const std::string& information)
{
	help new_argument;

	new_argument.short_param = short_operation;
	new_argument.param = operation;
	new_argument.information = information;

	Values_Storage.push_back(new_argument);

	return reinterpret_cast<ArgumentParser::ArgParser::help&>(Values_Storage[Values_Storage.size() - 1]);
}

bool ArgumentParser::ArgParser::Help()
{
	ArgumentParser::ArgParser::help* buff;

	bool res = false;

	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<help>(Values_Storage[i])) {
			buff = std::get_if<help>(&Values_Storage[i]);
			std::cout << buff->information;
			res = true;
		}
	}
	return res;
}

std::string ArgumentParser::ArgParser::HelpDescription()
{
	std::string help = parser_name + '\n';

	ArgumentParser::ArgParser::help* buff;
	ArgumentParser::ArgParser::args_string* buff_str;
	ArgumentParser::ArgParser::args_int* buff_int;
	ArgumentParser::ArgParser::args_bool* buff_bool;

	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<ArgumentParser::ArgParser::help>(Values_Storage[i])) {
			buff = std::get_if<ArgumentParser::ArgParser::help>(&Values_Storage[i]);
			help += buff->information + '\n';
			break;
		}
	}

	help += '\n';

	for (int i = 0; i < Values_Storage.size(); i++) {
		if (std::holds_alternative<ArgumentParser::ArgParser::args_string>(Values_Storage[i])) {
			buff_str = std::get_if<args_string>(&Values_Storage[i]);
			help += buff_str->short_param + ", --" + buff_str->param + "=<string>, " + buff_str->information + '\n';
		}
		else if (std::holds_alternative<ArgumentParser::ArgParser::args_int>(Values_Storage[i])) {
			buff_int = std::get_if<args_int>(&Values_Storage[i]);
			help += buff_int->short_param + ", --" + buff_int->param + "=<int>, " + buff_int->information + '\n';
		}
		else if (std::holds_alternative<ArgumentParser::ArgParser::args_bool>(Values_Storage[i])) {
			buff_bool = std::get_if<args_bool>(&Values_Storage[i]);
			help += buff_bool->short_param + ", --" + buff_bool->param + ", " + buff_bool->information + '\n';
		}
	}

	return std::string();
}
