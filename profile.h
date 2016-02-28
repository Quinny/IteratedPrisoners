#ifndef PROFILE_H
#define PROFILE_H

#include <chrono>
#include <map>
#include <string>
#include <iostream>

namespace qp {

// Run the function f(args) and return the execution time in seconds
// for bench marking bottle necks and quantifying speed ups
template <typename Func, typename... Args>
double time_operation(Func&& f, Args&&... args) {
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    f(args...);
    auto t2 = high_resolution_clock::now();
    return duration_cast<duration<double>>(t2 - t1).count();
}

enum class log_policy {
    loud,
    silent
};

class profiler {

private:
    static std::map<std::string, double> sections;
    static log_policy policy;

    std::chrono::time_point<std::chrono::high_resolution_clock> t1;
    std::chrono::time_point<std::chrono::high_resolution_clock> t2;
    bool done;

public:
    std::string tag;

    profiler(std::string t): tag(t),
        t1(std::chrono::high_resolution_clock::now()), done(false) {};

    void log() {
        if (profiler::policy == log_policy::loud)
            std::cout << tag << ":" << sections[tag] << std::endl;
    }

    void stop() {
        if (done) return;
        using namespace std::chrono;
        t2 = high_resolution_clock::now();
        sections[tag] += duration_cast<duration<double>>(t2 - t1).count();
        done = true;
        log();
    }

    static void dump() {
        for (auto i: sections) {
            std::cout << i.first << ": " << i.second << std::endl;
        }
    }

    ~profiler() {
        stop();
    }
};

log_policy profiler::policy = log_policy::silent;
std::map<std::string, double> profiler::sections = {};

}

#endif /* PROFILE_H */
