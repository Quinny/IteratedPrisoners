#include <vector>
#include <functional>
#include <iostream>
#include <list>
#include <iterator>

#include "common.h"
#include "bots.h"
#include "genetic.h"

int main() {
    using namespace ipd;
    auto gen_guy  = genetic::make_prisoner_t(genetic::genetic_strategy(4));
    auto ev_guy   = genetic::evolve(100, 30, 10);
    auto cp       = bots::all;


    //cp.push_back(gen_guy);
    cp.push_back(ev_guy);

    auto sb = play_tourny(cp, 100);
    std::sort(sb.begin(), sb.end(), [] (score_t x, score_t y) {
            return x.second > y.second;
    });
    for (auto i: sb) {
        std::cout << i.first << ", " << i.second << std::endl;
    }


    return 0;
}
