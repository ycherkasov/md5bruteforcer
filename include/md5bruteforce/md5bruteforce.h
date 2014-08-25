#ifndef MD5_BRUTEFORCER_H
#define MD5_BRUTEFORCER_H

#include <string>
#include <ctime> // clang unhappy about chrono+stdlibc, use clock() instead
#include <atomic>
#include "md5bruteforce/md5.h"


/// @brief MD5 multi-threaded brute-forcer
/// @note That version supports alphabetical symbols and digits only
/// and could be expanded, however it will cause exponent complexity growth
class md5bruteforcer{
public:

    /// @brief MD5 multi-threaded brute-forcer
    /// @param hash: hash to crack
    /// @param charset: charactes cotained in the password
    md5bruteforcer(const std::string& hash, const std::string& charset);

    /// @brief Decrypt password of provided length
    // by looking though all veriations with repetitions using existing charset
    /// @param min_length: minimal password length to try
    /// @param max_length: maximal password length to try
    std::string find_password(size_t min_length, size_t max_length);

    /// @brief Look through all variation using length given
    /// @param word_length: symbols in the password
    /// @param max_threads: override maximum number of threads
    /// that could be executed in the thresad pool
    /// @note: max_threads = 0 is default, means that pool size is equial number of CPU cores
    void guess_loop(size_t word_length, size_t max_threads = 0);

private:

    /// Time wrapper so that measure every iteration and total time
    struct calc_timer {
        std::clock_t _timestamp;
        calc_timer() :_timestamp(clock()){}
        double duration() {
            clock_t duration = (clock() - _timestamp);
            return static_cast<double>(duration) / CLOCKS_PER_SEC;
        }
    };


    /// Check whether all passed characters could be used in variation
    /// Throws exception if doesn't
    void check_charset();

    /// Just compare geterates hash and hash to check
    inline bool compare(const std::string& guess){
        return guess == _hash;
    }

    // copy of hash
    const std::string _hash;

    // charset used in attack
    const std::string _alphabet;

    // the whole charset
    static const std::string chars;

    // atomic flag to finish all threads
    std::atomic<bool> _password_found;

    // save found password here
    std::string _detected_password;
};

#endif // MD5_BRUTEFORCER_H
