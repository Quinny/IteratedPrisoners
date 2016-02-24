#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

namespace test {

const std::string reset = "\033[0m";
const std::string red   = "\033[31m";
const std::string green = "\033[32m";
const std::string cyan  = "\033[36m";
const std::string yellow= "\033[33m";

using test_fn = std::function<void()>;

void assert_true(bool b, std::string message, const char* file, const char* fn, int line) {
    static std::string last_fn;
    if (last_fn != fn)
        std::cout << cyan <<
            "-------------------------------------------------------"
        << reset << std::endl;
    last_fn = fn;
    if (!b) {
        std::cerr << red << "Test failed: " << file << " " << fn << " line " << line << std::endl;
        std::cerr << message << reset << std::endl;
    }
    else {
        std::cerr << green << fn << " \"" << message << "\" passed!" << reset << std::endl;
    }
}

#define assert_true(b, s) assert_true(b, s, __FILE__, __FUNCTION__, __LINE__)

void run(test_fn f) {
    f();
}

template <typename T>
void run_all(T first, T last) {
    std::for_each(first, last, run);
}

}

#endif /* TEST_H */
