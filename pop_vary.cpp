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
    }
    return total / n;
}

void set_popsize(int n) {
    using namespace ipd::config;
    pop_size = n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    for (int i = 2; i < 1000; i += 50) {
        set_popsize(i);
        auto avg = average_using(ipd::genetic::evaluate_vs_tft, 10);
        std::cout << ipd::config::pop_size << " " << avg << std::endl;
    }
    return 0;
}
