#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "common.h"
#include "bots.h"
#include "genetic.h"
#include "profile.h"

void log(const ipd::prisoner_t& p, int score) {
    std::fstream f("winners.txt", std::ios_base::app);
    f << p << " " << score << std::endl;
}

void ev_against_classic() {
    using namespace ipd;
    auto ev_guy   = genetic::evolve(100, 2, 1000, genetic::evaluate_vs_tft);
    auto cp       = bots::all;
    cp.push_back(ev_guy);

    auto sb = play_tourny(cp, 100);
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

    auto sb = play_tourny(winners, 100);
    for (auto i: sb)
        std::cout << i.first << ", " << i.second << std::endl;
}

double average_using(ipd::genetic::fitness_fn fn, int n) {
    double total = 0;
    for (int i = 0; i < n; ++i) {
        auto guy = ipd::genetic::evolve(1000, 2, 100, fn);
        auto score = ipd::total_score(guy, ipd::bots::all, 100);
        total += score;
        log(guy, score);
    }
    return total / n;
}

int main() {
    auto a1 = average_using(ipd::genetic::evaluate_async, 100);
    std::cout << "multibot eval average " << a1 << std::endl;

    auto a2 = average_using(ipd::genetic::evaluate_vs_tft, 100);
    std::cout << "single tft bot eval average " << a2 << std::endl;
    return 0;
}
