#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include "md5bruteforce/thread_pool.h"
#include "md5bruteforce/md5bruteforce.h"
#include "md5bruteforce/utils.h"


// unsigned int64 conversion to string is not standardized, so implement C-style local portable version

namespace{

char* ulltoa(long long value, char *s, int radix, const char* chars) {

    char temp[255];
    char	*p, *q;

    if (!s){
        return s;
    }

    /* build the string in reverse order... */
    p = temp;

    /* becomes null terminator later */
    *p++ = '\0';

    do{
        *p++ = chars[value % radix];
        value /= radix;
    } while (value);

    /* reverse the string into the caller's string */
    q = (char*)s;
    /*double braces to suppress clang warning*/
    while ( *q++ = *--p);

    return s;
}

}


using namespace std;

const string md5bruteforcer::chars("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

md5bruteforcer::md5bruteforcer(const string& hash, const string& charset)
    :_hash(hash),
    _alphabet(charset),
    _password_found(false){}


std::string md5bruteforcer::find_password(size_t min_length, size_t max_length){

    calc_timer t;

    // throws stdf::runtime_error if does not comply with charset
    check_charset();

    for (size_t sz = min_length; sz <= max_length; ++sz){
        calc_timer t1;
        guess_loop(sz);
        if (_password_found) {
            break;
        }
        cout << sz << "-symbols combinations processed in ";
        cout << fixed << setprecision(2) << t.duration() << " sec\n";
    }
    // Report
    cout << fixed << setprecision(2) << "Completed in " << t.duration() << " sec\n";
    return _detected_password;
}


void md5bruteforcer::check_charset(){
    for (char c : _alphabet) {
        // check whether symbol exists in charset
        if (0 == count(chars.begin(), chars.end(), c)){
            stringstream err;
            err << "Supported charset is " << chars << endl;
            throw std::runtime_error(err.str().c_str());
        }

    }

}


void md5bruteforcer::guess_loop(size_t word_length, size_t max_threads/* = 0*/) {

    int alphabet_length = _alphabet.size();

    // number of CPU cores
    size_t number_of_threads = thread::hardware_concurrency();

    unsigned long long total_combinations = pow(alphabet_length, word_length);

    // realtively small number of combinations, 1 thread is enough
    if (total_combinations < 1024 * 8){
        number_of_threads = 1;
    }
    // optimization
    else if ((total_combinations < 1024 * 16) && (number_of_threads > 2)){
        number_of_threads /= 2;
    }

    // if number of threads is set expticitly
    if ((max_threads) && (number_of_threads > max_threads)) {
        number_of_threads = max_threads;
    }

    // Select frame size so that enque to the threadpool
    long start_combination = 0, end_combination = total_combinations;
    long frame = total_combinations / number_of_threads;

    thread_pool pool(number_of_threads);

    // iterate over all veriations with repetitions
    for (unsigned long long next_combination = start_combination;
        next_combination <= end_combination;
        next_combination += frame) {

        auto check_frame = [this, start_combination, next_combination, alphabet_length, word_length]{

            // iterate over selected frame
            for (unsigned long long i = start_combination; i < next_combination; ++i) {

                if (_password_found)
                    return;

                md5 md5;
                char buf[255] = {};
                string str(ulltoa(i, buf, alphabet_length, chars.c_str()));

                // align to 'zero symbol'
                string result;
                while (result.size() + str.size() < word_length)
                    result += _alphabet[0];

                for_each(str.begin(), str.end(), [&result, this](char c){
                    size_t index_of = std::string::npos;
                    if (std::string::npos != (index_of = chars.find(c))){
                        result += _alphabet[index_of];
                    }
                    else{
                        // should not be here. Could not throw exception in separate thread
                        cerr << "ERROR: Out of alphabet" << endl;
                        return;
                    }
                });
                
                std::string new_hash(md5.digest_string(result.c_str()));

                if (compare(new_hash)){
                    _password_found = true;
                    _detected_password = result;
                    return;
                }
            }
        };

        pool.enqueue(check_frame);
    }
}
