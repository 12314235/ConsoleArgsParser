#pragma once
#include <string>
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

namespace ArgumentParser {

class ArgParser {
protected:
    struct args_int {
        std::string param;
        std::string short_param;

        bool is_multi_value = false;
        bool is_positional = false;
        bool is_used = false;

        std::string information = "";

        size_t min_args_count;
        std::vector<int> int_values;
        std::vector<int>* int_values_ptr = nullptr;
        int int_value;
        int* int_value_ptr = nullptr;

        args_int& MultiValue() {
            is_multi_value = true;
            min_args_count = 0;
            return *this;
        }

        args_int& MultiValue(size_t i) {
            is_multi_value = true;
            min_args_count = i;
            return *this;
        }

        args_int& StoreValue(int& storage) {
            int_value_ptr = &storage;
            return *this;
        }

        args_int& StoreValues(std::vector<int>& storage) {
            if (!is_multi_value) {
                throw;
            }
            int_values_ptr = &storage;

            return *this;
        }

        args_int& Positional() {
            is_positional = true;
            return *this;
        }
    };

    struct args_string {
        std::string param;
        std::string short_param;
 
        std::string string_value = "";
        std::string* string_value_ptr = nullptr;

        std::string information = "";

        bool is_used = false;
        bool is_multi_value = false;

        int min_args_count;

        args_string& MultiValue() {
            is_multi_value = true;
            min_args_count = 0;
            return *this;
        }

        args_string& MultiValue(size_t i) {
            is_multi_value = true;
            min_args_count = i;
            return *this;
        }

        args_string& StoreValue(std::string& storage) {
            string_value_ptr = &storage;
            return *this;
        }

        args_string& Default(const std::string& value) {
            string_value = value;
            is_used = true;

            return *this;
        }
    };

    struct args_bool {
        std::string param;
        char short_param;

        std::string information = "";

        bool is_used = false;

        bool bool_value = false;
        bool* bool_value_ptr = nullptr;

        args_bool& StoreValue(bool& storage) {
            bool_value_ptr = &storage;
            return *this;
        }

        args_bool& Default(bool value) {
            bool_value = value;
            information += value ? " [default=true]\n" : " [default=false]";
            is_used = true;
            return *this;
        }
    };

    struct help {
        char short_param;
        std::string param;

        bool is_used = true;

        std::string information;
    };
    

public:
    std::vector<std::variant<args_int, args_string, args_bool, help>> Values_Storage;
    std::string parser_name;

    ArgParser(const std::string& name);

    bool Parse(const std::vector<std::string>& argv);
    bool Parse(int argc, char** argv);

    args_string& AddStringArgument(const std::string& param);
    args_string& AddStringArgument(char short_param, const std::string& param);
    args_string& AddStringArgument(char short_param, const std::string& param, const std::string& information);

    args_int& AddIntArgument(const std::string& param);
    args_int& AddIntArgument(char short_param, const std::string& param);
    args_int& AddIntArgument(const std::string& param, const std::string& information);

    args_bool& AddFlag(char flag, const std::string& name);
    args_bool& AddFlag(char flag, const std::string& name, const std::string& information);
    args_bool& AddFlag(const std::string& name, const std::string& information);

    std::string GetStringValue(const std::string& param);

    int GetIntValue(const std::string& param);
    int GetIntValue(const std::string& param, int index);

    bool GetFlag(const std::string name);

    help& AddHelp(char short_operation, const std::string& operation, const std::string& information);

    bool Help();

    std::string HelpDescription();
};

} 