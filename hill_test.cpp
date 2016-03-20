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

void hill_against_classic() {
    using namespace ipd;
    auto hill_guy = hill_climb::climb(
        make_prisoner_t(genetic::genetic_strategy("cccccdcdccdddcdcddccddccdccddcdccdccdcdcdcdcddcdcccdcccccdddcccc")),
        config::mutation_rate,
        config::hill_pop
    );
    auto cp       = bots::all;
    cp.push_back(hill_guy);

    auto sb = play_tourny(cp, config::rounds);
    for (auto i: sb)
        std::cout << i << std::endl;
}

void hill_climb_test(int hill_pop) {
    using namespace ipd;
    qp::profiler prof("pop " + std::to_string(hill_pop));

    double score = 0;
    int runs = 1;

    for (int i = 0; i < runs; ++i) {
        auto hill_guy = hill_climb::climb(
            make_prisoner_t(genetic::genetic_strategy("ccdcdccdcccddddccdcdddcccccccddccdddddccccddccdddcccdddcccdcddcc")),
            config::mutation_rate,
            hill_pop
        );

        auto cp = bots::all;
        cp.push_back(hill_guy);

        score += total_score(hill_guy, cp, config::rounds);
        std::cout << hill_guy.name << std::endl;
    }

    prof.stop();
    std::cout << hill_pop << " " << score / runs << std::endl;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    hill_climb_test(1000000);

    return 0;
}
