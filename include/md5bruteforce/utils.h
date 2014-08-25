#ifndef MD5_UTILS_H
#define MD5_UTILS_H


#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>


/// @brief Utility class with methods used to pase applications params
struct brute_command_parser{

    /// @brief Exception reports that unable to convert string to numeric type
    class string_cast_error : std::runtime_error{
    public:
        string_cast_error(const char* what) : runtime_error(what){}
        virtual const char* what() const _NOEXCEPT { return runtime_error::what(); }
    };

    /// @brief convert provided string to any (particualrry numeric) type
    /// @param str: string to convert
    template<typename T>
    static T string_to_any(const std::string& str) {
        T retval;
        std::stringstream stream(str);
        stream >> retval;
        if (stream.fail()) {
            std::stringstream err;
            err << "Failed to convert "
                << str
                << " to the type provided\n";
            throw string_cast_error(err.str().c_str());
        }
        return retval;
    }

    /// @brief Split (tokenize) string used provided delometer symbol
    /// For example: aaa:bbb:ccc with tokenizer ':' is splitted to a vector {'aaa','bbb','ccc'}
    /// @param str: string to tokenize
    /// @param token: tokenizer symbol (delimeter)
    /// @param out_tokens: vector to place received tokens
    static void tokenize(const std::string& str, const char token, std::vector<std::string>& out_tokens){
        std::istringstream f(str);
        std::string s;
        while (std::getline(f, s, token)) {
            out_tokens.push_back(s);
        }

    }
};



#endif //MD5_UTILS_H
