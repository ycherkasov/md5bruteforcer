#ifndef MD5_BRUTEFORCER_H
#define MD5_BRUTEFORCER_H

#include <string>
#include <chrono>
#include <atomic>
#include "md5bruteforce\md5.h"

// brute-forcer loop
class md5bruteforcer{
public:


	md5bruteforcer(const std::string& hash, const std::string& charset);

	std::string find_password(size_t min_length, size_t max_length);

private:

	// time wrapper so that measure total time
	struct calc_timer {
		typedef std::chrono::microseconds mcs;
		typedef std::chrono::high_resolution_clock clock;
		std::chrono::system_clock::time_point _timestamp;
		calc_timer() :_timestamp(clock::now()){}
		double duration() {
			auto duration = std::chrono::duration_cast<mcs>(clock::now() - _timestamp);
			return static_cast<double>(duration.count()) / 1000000.0;
		}
	};

	void guess_loop(size_t wordLength);

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