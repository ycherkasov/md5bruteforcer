#include <iostream>
#include <string>
#include <vector>
#include "md5bruteforce/md5bruteforce.h"
#include "md5bruteforce/utils.h"

using namespace std;

void test_assert(bool expr, unsigned line){
    if (!expr){
        cerr << "Test failed at line " << line << endl;
    }
}

void tokenize_test(){
    string tokens_str1("aaa;bbb;ccc");
    vector<string> tokens;
    brute_command_parser::tokenize(tokens_str1, ';', tokens);

    test_assert(tokens.size() == 3, __LINE__);
}

void string_conversion_test(){
    int i1 = 100;
    string si1("100");
    int i_check = brute_command_parser::string_to_any<int>(si1);
    test_assert(i_check == i1, __LINE__);

    double d1 = 1.5;
    string sd1("1.5");
    double d_check = brute_command_parser::string_to_any<double>(sd1);
    test_assert(d_check == d1, __LINE__);
}

void bruteforce_functional1(){
    string hash("827ccb0eea8a706c4c34a16891f84e7b");
    string charset("0123456789");
    md5bruteforcer brute(hash, charset);
    std::string password = brute.find_password(3, 7);
    test_assert(password == "12345", __LINE__);
}

void bruteforce_functional2(){
    string hash("d3eb9a9233e52948740d7eb8c3062d14");
    string charset("0123456789");
    md5bruteforcer brute(hash, charset);
    std::string password = brute.find_password(3, 7);
    test_assert(password == "99999", __LINE__);
}

int main(int argc, char* argv[]){

    tokenize_test();
    string_conversion_test();
    bruteforce_functional1();
    bruteforce_functional2();
    return 0;
}

