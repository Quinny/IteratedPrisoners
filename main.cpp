#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "ipd/common.h"
#include "ipd/bots.h"
#include "ipd/genetic.h"
#include "ipd/profile.h"
#include "ipd/config.h"

void log(const ipd::prisoner_t& p, int score) {
    std::fstream f("winners.txt", std::ios_base::app);
    f << p << " " << score << std::endl;
}

void ev_against_classic() {
    using namespace ipd;
    auto ev_guy   = genetic::evolve(
        ipd::config::pop_size,
        ipd::config::mutation_rate,
        ipd::config::generations,
        genetic::evaluate_vs_tft
    );
    auto cp       = bots::all;
    cp.push_back(ev_guy);

    auto sb = play_tourny(cp, ipd::config::rounds);
    for (auto i: sb)
        std::cout << i << std::endl;

    log(sb.front().first, sb.front().second);
}

void winner_battle() {
    using namespace ipd;

    std::string genome;
    int n;
    std::ifstream f("winners.txt");
    std::vector<prisoner_t> winners;

    while (f >> genome >> n)
        winners.emplace_back(
            make_prisoner_t(genetic::genetic_strategy(genome))
        );

    auto sb = play_tourny(winners, ipd::config::rounds);
    for (auto i: sb)
        std::cout << i.first << ", " << i.second << std::endl;
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
        auto score = ipd::total_score(guy, ipd::bots::all, ipd::config::rounds);
        total += score;
        log(guy, score);
    }
    return total / n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    ev_against_classic();
    return 0;
}
