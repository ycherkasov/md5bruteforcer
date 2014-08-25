#include <iostream>
#include <algorithm>
#include <cctype>

#include "md5bruteforce/md5.h"
#include "md5bruteforce/md5bruteforce.h"
#include "md5bruteforce/utils.h"

using namespace std;

/*
Simple multithreaded MD5 hash bruteforcer

Examples of usage:

Hash for '12345'
brute 3 5 48-57 827ccb0eea8a706c4c34a16891f84e7b

Hash for '99999'
brute 3 5 48-57 d3eb9a9233e52948740d7eb8c3062d14

Hash for 'querty'
brute 3 6 97-122 356d0f282e7da14cb73f2614d191933b

Hash for 'password'
brute 3 6 97-122 5f4dcc3b5aa765d61d8327deb882cf99
*/


/// @brief Print usage end exit correctly (with calling global destructors)
void usage_exit(){
    cerr << "Usage: brute <minimal-symbols-number> <maximal-symbols-number> <ASCII-range>:...:<ASCII-range> <hash-to-check>" << endl;
    cerr << "Example: brute.exe 3 10 65-90:97-122 5f4dcc3b5aa765d61d8327deb882cf99" << endl;
    cerr << "Form 3 to 10 symbols, using charsets [a-z]+[A-Z]" << endl;
    exit(0);
}

/// @brief Compose a string containing charset that could be used for guessing a password
/// @param ranges_str: string which has "<num>-<num>:<num>-<num>:...:<num>-<num>" format
/// For example 65-90:97-122, which is all lower-case and upper case symbols
/// @return std::string with all possible symbols contained in password
string compose_charset(const char* ranges_str){

    vector<string> ranges;
    string symbols;
    symbols.reserve(255);
    brute_command_parser::tokenize(ranges_str, ':', ranges);

    // append symbols from every range
    for (auto &range : ranges){
        vector<string> minmax;
        brute_command_parser::tokenize(range, '-', minmax);
        if (minmax.size() != 2){
            usage_exit();
        }
        // fetch range from string
        size_t min_range = brute_command_parser::string_to_any<size_t>(minmax[0]);
        size_t max_range = brute_command_parser::string_to_any<size_t>(minmax[1]);

        // swap range is min>max
        if (min_range > max_range){
            size_t tmp = min_range;
            min_range = max_range;
            max_range = tmp;
        }

        // +1 so that create [] range, not [)
        size_t length = max_range - min_range + 1;

        // generate a symbols vector from range (i.e. v = {65, 66, ...,60} for passed "65-90" string)
        size_t incrementor = min_range;
        vector<unsigned char> symbols_range(length);
        generate(symbols_range.begin(), symbols_range.end(),
            [&incrementor]() mutable {return static_cast<unsigned char>(incrementor++); });

        // append to all symbols set
        symbols.insert(symbols.begin(), symbols_range.begin(), symbols_range.end());
    }

    // sort and remove duplications if exist
    sort(symbols.begin(), symbols.end());
    symbols.erase(unique(symbols.begin(), symbols.end()), symbols.end());
    return symbols;
}


int main(int argc, char* argv[]){

    // commandd-line params order
    static const size_t min_length_param = 1;
    static const size_t max_length_param = 2;
    static const size_t charset_param = 3;
    static const size_t hash_param = 4;

    if (argc != 5) {
        usage_exit();
    }

    size_t min_length = 0, max_length = 0;
    string charsets;
    string hash_to_check(argv[hash_param]);

    // check wthether hash length is correct and symbols are hexidecimal
    if ((32 != hash_to_check.size())
        || 32 != (count_if(hash_to_check.begin(), hash_to_check.end(), ::isxdigit))) {
        cerr << "MD5 hash should consist of 32 hexidecimal numbers" << endl;
        usage_exit();
    }

    // parse command line numeric params
    try {
        min_length = brute_command_parser::string_to_any<size_t>(argv[min_length_param]);
        max_length = brute_command_parser::string_to_any<size_t>(argv[max_length_param]);
        charsets = compose_charset(argv[charset_param]);
    }
    catch (const brute_command_parser::string_cast_error& e) {
        cerr << e.what() << endl;
        usage_exit();
    }

    // find password
    try{
        md5bruteforcer brute(hash_to_check, charsets);
        std::string password = brute.find_password(min_length, max_length);

        if (!password.empty()){
            cout << "The password has been found: " << password << endl;
        }
        else{
            cout << "Password not found, try expanding charset or password lengths range" << password << endl;
        }

    }
    catch (const std::exception& e){
        cerr << "Error occured: " << e.what() << endl;
    }

    return 0;
}
