#ifndef GENETIC_H
#define GENETIC_H

#include "common.h"

// TODO
// - actual genetic part
// - maybe use total_score from common as fitness
// - function which creates prisoner_t from a genetic_strat
//      * auto fills out name as strategy vector
// - function which takes strategy string ("cdccddc..") and creates a strategy

struct genetic_strategy {
    history_t history;
    // len == 4 ^ memsize
    std::vector<decision> strategy;
    std::size_t mem_size;

    genetic_strategy(int n): mem_size(n) {}

    // TODO
    // this.
    std::size_t encode(history_t op_history) {
        (void)op_history;
        return 0UL;
    }

    decision operator () (history_t op_history) {
        auto d = strategy[encode(op_history)];
        history.push_front(d);
        return d;
    }
};

#endif /* GENETIC_H */
