#include <iostream>
#include <string>
#include <vector>
#include "md5bruteforce\md5bruteforce.h"
#include "md5bruteforce\utils.h"

using namespace std;

void test_assert(bool expr, unsigned line){
	if (!expr){
		cerr << "Test failed at line " << line << endl;
		exit(0);
	}
}

void tokenize_test(){
	string tokens_str1("aaa;bbb;ccc");
	vector<string> tokens;
	tokenize(tokens_str1, ';', tokens);

	test_assert(tokens.size() == 3, __LINE__);
	test_assert(tokens[0] == "aaa", __LINE__);
	test_assert(tokens[1] == "bbb", __LINE__);
	test_assert(tokens[2] == "ccc", __LINE__);
}

void string_conversion_test(){
	int i1 = 100;
	string si1("100");
	int i_check = string_to_any<int>(si1);
	test_assert(i_check == i1, __LINE__);

	double d1 = 1.5;
	string sd1("1.5");
	double d_check = string_to_any<double>(sd1);
	test_assert(d_check == d1, __LINE__);
}

int main(int argc, char* argv[]){
	tokenize_test();
	string_conversion_test();
    return 0;
}
 
