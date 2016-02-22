#ifndef GENETIC_H
#define GENETIC_H

#include <cmath>
#include <random>
#include <ctime>
#include <functional>

#include "common.h"

// TODO
// - actual genetic part
// - maybe use total_score from common as fitness

decision random_decision() {
    static std::uniform_int_distribution<int> dist(0, 1);
    static std::knuth_b gen (std::time(nullptr));
    static auto roll = std::bind(dist, gen);

    if (roll())
        return decision::cooperate;
    return decision::defect;
}

struct genetic_strategy {
    history_t history;
    std::vector<decision> strategy;
    std::size_t mem_size;

    // Assigns a random strategy
    genetic_strategy(const std::size_t n) :
        strategy(std::pow(4, n)), mem_size(n) {
        std::generate(strategy.begin(), strategy.end(), random_decision);
    }

    // Construct a strategy given a genome string
    genetic_strategy(const std::string& genome) :
        strategy(genome.size()), mem_size(genome.size())
    {
        auto f = [] (char c) {
            if (c == 'c') return decision::cooperate;
            return decision::defect;
        };
        std::transform(genome.begin(), genome.end(), strategy.begin(), f);
    }

    // Encodes the history of both players into an integer in the form of
    // (my last move) (op's last move) ... (my oldest move) (op's oldest move)
    // Coop = 0 bit
    // Defect = 1 bit

    /*
     * op_history must be the same size as history or the encoding will not
     * work, play function must keep track of all moves to avoid conflicts
     */
    std::size_t encode(const history_t& op_history) const {
        std::size_t code = 0;

        auto my_iter = history.begin();
        auto op_iter = op_history.begin();

        for (auto shift = 0UL; shift < 2 * mem_size; shift += 2) {
            if (my_iter == history.end() || op_iter == op_history.end())
                break;

            if (*my_iter == decision::cooperate)
                code |= 1 << shift;

            if (*op_iter == decision::cooperate)
                code |= 1 << (shift + 1);

            ++my_iter; ++op_iter;
        }
        return code;
    }

    decision operator () (const history_t& op_history) {
        auto d = strategy[encode(op_history)];
        history.push_front(d);
        return d;
    }
};

// Create a prisoner_t given a genetic strategy, where the name is a string
// representing it's strategy
prisoner_t make_prisoner_t(const genetic_strategy& gs) {
    std::string name = "";
    for (decision d : gs.strategy) {
        if (d == decision::cooperate)
            name += 'c';
        else
            name += 'd';
    }
    return {name, gs};
}

#endif /* GENETIC_H */
