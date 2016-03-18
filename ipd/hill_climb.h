#ifndef HILL_CLIMB_H
#define HILL_CLIMB_H

#include "config.h"
#include "genetic.h"

namespace ipd {

namespace hill_climb {

using namespace std::placeholders;

// generate a list of successors using random mutation
std::vector<prisoner_t>
mutation_successor(const prisoner_t& p, const int rate) {
    std::vector<prisoner_t> ret;
    auto mutation = std::bind(genetic::mutate_strategy, _1, rate);

    for (int i = 0; i < config::pop_size; ++i) {
        auto n = mutation(p.name);
        ret.push_back(make_prisoner_t(genetic::genetic_strategy(n)));
    }

    return ret;
}

// Returns a new prisoner_t that is a mutation of the original
prisoner_t mutate(const prisoner_t& p, const int rate) {
    auto mutation = std::bind(genetic::mutate_strategy, _1, rate);
    auto n = mutation(p.name);
    return make_prisoner_t(genetic::genetic_strategy(n));
}

// TODO
// potentially different selection function
//      - add some randomness
//      - sideway movement
//      - etc.
// and different way to deal with reaching local max
//      - random restart
//      - etc.

// Given an initial prisoner, use a hill-climbing approach to try to generate
// a strong IPD candidate
prisoner_t climb(prisoner_t init, const int rate,
        genetic::fitness_fn fitness_func) {
    while (true) {
        auto succ = mutation_successor(init, rate);
        succ.push_back(init);

        auto evaluation = fitness_func(succ);
        auto m = std::max_element(evaluation.begin(), evaluation.end(),
                std::less<score_t>());

        // best guy was initial guy
        if (m->first.name == init.name)
            break;
        init = m->first;
    }

    return init;
}

int single_vs_tft(const prisoner_t& p) {
    prisoner_t tft = {"", bots::tft};
    return play(p, tft, config::rounds);
}

prisoner_t climb(prisoner_t init, const int rate, int rounds) {
    for (int i = 0; i < rounds; ++i) {
        auto succ = mutate(init, rate);
        if (single_vs_tft(succ) > single_vs_tft(init))
            init = succ;
    }
    return init;
}

prisoner_t restart_climb(
    prisoner_t init,
    const int rate,
    genetic::fitness_fn fitness_func,
    int restarts
) {
    std::vector<prisoner_t> candidates;

    do {
        candidates.push_back(climb(init, rate, fitness_func));
        init = genetic::random_prisoner_t(config::mem_first, config::mem_last);
        --restarts;
    } while (restarts > 0);

    auto eval = fitness_func(candidates);
    auto max = std::max_element(eval.begin(), eval.end(), std::less<score_t>());
    return max->first;
}

};

};

#endif /* HILL_CLIMB_H */
