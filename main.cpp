#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "common.h"
#include "bots.h"
#include "genetic.h"
#include "profile.h"

struct score_compare {
    bool operator () (const ipd::score_t& x, const ipd::score_t& y) const {
        return x.second > y.second;
    }
};

void ev_against_classic() {
    using namespace ipd;
    auto ev_guy   = genetic::evolve(1000, 2, 30, genetic::evaluate_vs_tft);
    auto cp       = bots::all;
    cp.push_back(ev_guy);

    auto sb = play_tourny(cp, 100);
    std::sort(sb.begin(), sb.end(), score_compare());

    for (auto i: sb)
        std::cout << i << std::endl;

    // append winner to a text file for later analysis
    std::fstream f("winners.txt", std::ios_base::app);
    f << sb.front().first << " " << sb.front().second << std::endl;
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

    for (const auto& i: bots::all)
        winners.push_back(i);

    auto sb = play_tourny(winners, 100);
    std::sort(sb.begin(), sb.end(), score_compare());
    for (auto i: sb)
        std::cout << i.first << ", " << i.second << std::endl;
}

int main() {
    qp::profiler p("entire thing");
    ev_against_classic();
    //winner_battle();
    std::cout << "--------" << std::endl;
    p.stop();
    qp::profiler::dump();
    return 0;
}
