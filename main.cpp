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

void ev_against_classic() {
    using namespace ipd;
    auto ev_guy   = genetic::evolve(
        config::pop_size,
        config::mutation_rate,
        config::generations,
        genetic::evaluate_vs_tft
    );
    auto cp       = bots::all;
    cp.push_back(ev_guy);

    auto sb = play_tourny(cp, config::rounds);
    for (auto i: sb)
        std::cout << i << std::endl;

    log(sb.front().first, sb.front().second);
}

void hill_against_classic() {
    using namespace ipd;
    auto hill_guy = hill_climb::climb(
        make_prisoner_t(genetic::genetic_strategy(3)),
        config::mutation_rate,
        genetic::evaluate_vs_tft
    );
    auto cp       = bots::all;
    cp.push_back(hill_guy);

    auto sb = play_tourny(cp, config::rounds);
    for (auto i: sb)
        std::cout << i << std::endl;

    //log(sb.front().first, sb.front().second);
}

void restart_hill_against_classic(int restarts) {
    using namespace ipd;
    auto hill_guy = hill_climb::restart_climb(
        genetic::random_prisoner_t(config::mem_first, config::mem_last),
        config::mutation_rate,
        genetic::evaluate_vs_tft,
        restarts
    );
    auto cp = bots::all;
    cp.push_back(hill_guy);

    auto sb = play_tourny(cp, config::rounds);
    for (auto i: sb)
        std::cout << i << std::endl;

    // log(sb.front().first, sb.front().second);
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

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    ev_against_classic();
    //hill_against_classic();
    //restart_hill_against_classic(1000);
    return 0;
}
