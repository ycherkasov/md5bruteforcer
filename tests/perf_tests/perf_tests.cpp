#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include "md5bruteforce/md5bruteforce.h"
#include "md5bruteforce/utils.h"

using namespace std;

/// Time wrapper
struct calc_timer {
    std::clock_t _timestamp;
    calc_timer() :_timestamp(clock()){}
    double duration() {
        clock_t duration = (clock() - _timestamp);
        return static_cast<double>(duration) / CLOCKS_PER_SEC;
    }
};

// some reference password check iteration in one thread
void iteration(){
    string hash("827ccb0eea8a706c4c34a16891f84e7b");
    string charset("0123456789");
    md5bruteforcer brute(hash, charset);
    brute.guess_loop(5, 1);
}

template <typename T>
void performance_test(T func, size_t iters){

    std::vector<double> v;
    v.reserve(iters);

    cout << "Performing " << iters << " iterations..." << endl;
    for (size_t i = 0; i < iters; ++i){
        calc_timer t;
        func();
        v.push_back(t.duration());
    }

    // calculate average
    double total = accumulate<vector<double>::iterator, double>(
        v.begin(),
        v.end(),
        0, std::plus<double>());

    double avg = total / iters;

    // calculate median
    size_t middle = iters / 2;
    nth_element(v.begin(), v.begin() + middle, v.end());
    double med = v[middle];
    cout << "Average time of reference test is " << avg << endl;
    cout << "Median time of reference test is " << med << endl;
}

int main(int argc, char* argv[]){
    performance_test(iteration, 100);
    return 0;
}

