#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "ipd/common.h"
#include "ipd/bots.h"
#include "ipd/genetic.h"
#include "ipd/profile.h"
#include "ipd/config.h"
#include "ipd/hill_climb.h"

void log(const ipd::prisoner_t& p, int score) {
    std::fstream f("winners.txt", std::ios_base::app);
    f << p << " " << score << std::endl;
}

double average_using(ipd::genetic::fitness_fn fn, int n) {
    double total = 0;
    for (int i = 0; i < n; ++i) {
        auto guy = ipd::genetic::evolve(
            ipd::config::pop_size,
            ipd::config::mutation_rate,
            ipd::config::rounds,
            fn
        );
        auto cp = ipd::bots::all;
        cp.push_back(guy);
        auto score = ipd::total_score(guy, cp, ipd::config::rounds);
        total += score;
        log(guy, score);
    }
    return total / n;
}

void set_memsize(int n) {
    using namespace ipd::config;
    mem_first = mem_last = n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    set_memsize(1);
    qp::profiler ms1("memsize 1");
    auto avg_ = average_using(ipd::genetic::evaluate_vs_tft, 50);
    ms1.stop();
    qp::profiler::dump();
    std::cout << "memsize 1 avg score " << avg_ << std::endl;

    set_memsize(2);
    qp::profiler ms2("memsize 2");
    auto avg0 = average_using(ipd::genetic::evaluate_vs_tft, 50);
    ms2.stop();
    std::cout << "memsize 2 avg score " << avg0 << std::endl;

    set_memsize(3);
    qp::profiler ms3("memsize 3");
    auto avg1 = average_using(ipd::genetic::evaluate_vs_tft, 50);
    ms3.stop();
    std::cout << "memsize 3 avg score " << avg1 << std::endl;

    set_memsize(4);
    qp::profiler ms4("memsize 4");
    auto avg2 = average_using(ipd::genetic::evaluate_vs_tft, 50);
    ms4.stop();
    std::cout << "memsize 4 avg score " << avg2 << std::endl;

    set_memsize(5);
    qp::profiler ms5("memsize 5");
    auto avg3 = average_using(ipd::genetic::evaluate_vs_tft, 50);
    ms5.stop();
    std::cout << "memsize 5 avg score" << avg3 << std::endl;

    qp::profiler::dump();

    return 0;
}
