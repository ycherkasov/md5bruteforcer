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
    virtual const char* what() const noexcept { return runtime_error::what(); }
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

void strreverse(char* begin, char* end) {

    char aux;

    while(end>begin)

        aux=*end, *end--=*begin, *begin++=aux;

}

void itoa(int value, char* str, int base) {

    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    char* wstr=str;

    int sign;

    div_t res;



    // Validate base

    if (base<2 || base>35){ *wstr='\0'; return; }



    // Take care of sign

    if ((sign=value) < 0) value = -value;



    // Conversion. Number is reversed.

    do {

        res = div(value,base);

        *wstr++ = num[res.rem];

    }while(value=res.quot);

    if(sign<0) *wstr++='-';

    *wstr='\0';



    // Reverse string

    strreverse(str,wstr-1);

}
