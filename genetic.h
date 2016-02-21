#ifndef GENETIC_H
#define GENETIC_H

#include "common.h"

struct genetic_strategy {
    history_t history;
    // len == 4 ^ memsize
    std::vector<decision> strategy;
    std::size_t mem_size;

    genetic_strategy(int n): mem_size(n) {}

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
