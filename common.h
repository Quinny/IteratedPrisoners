#ifndef QPIPD_COMMON_H
#define QPIPD_COMMON_H

#include "ipd/genetic.h"
#include "ipd/config.h"

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


#endif /* COMMON_H */
