#ifndef MD5_UTILS_H
#define MD5_UTILS_H
#endif //MD5_UTILS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class string_cast_error : std::runtime_error{
public:
	string_cast_error(const char* what) : runtime_error(what){}
	virtual const char* what() const { return runtime_error::what(); }
};



template<typename T>
T string_to_any(const std::string& str) {
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


void tokenize(const std::string& str, const char token, std::vector<std::string>& out_tokens){

    std::istringstream f(str);
    std::string s;
    while (std::getline(f, s, token)) {
        out_tokens.push_back(s);
    }

}
