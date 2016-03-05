#ifndef PROFILE_H
#define PROFILE_H

#include <chrono>
#include <map>
#include <string>
#include <iostream>

namespace qp {

enum class log_policy {
    loud,
    silent
};

class profiler {

private:
    static std::map<std::string, double> sections;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;
    std::chrono::time_point<std::chrono::high_resolution_clock> t2;
    bool done;

public:
    std::string tag;
    static log_policy policy;

    profiler(std::string t): t1(std::chrono::high_resolution_clock::now()),
         done(false), tag(t) {};

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
std::map<std::string, double> profiler::sections{};

}

#endif /* PROFILE_H */
