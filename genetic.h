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

struct genetic_strategy {
    history_t history;
    // len == 4 ^ memsize
    std::vector<decision> strategy;
    std::size_t mem_size;

    // Assigns a random strategy
    genetic_strategy(const int n) :
        strategy(std::pow(4, n)), mem_size(n)
    {
        std::uniform_int_distribution<int> dist(0, 1);
        std::knuth_b gen;
        gen.seed(time(NULL));
        auto roll = std::bind(dist, gen);

        for (std::size_t i = 0; i != mem_size; ++i) {
            if (roll())
                strategy[i] = decision::cooperate;
            else
                strategy[i] = decision::defect;
        }
    }

    // Construct a strategy given a genome string
    genetic_strategy(const std::string& genome) :
        strategy(genome.size()), mem_size(genome.size())
    {
        for (std::size_t i = 0; i != mem_size; ++i) {
            if (genome[i] == 'c')
                strategy[i] = decision::cooperate;
            else
                strategy[i] = decision::defect;
        }
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

        for (
            int shift = 2 * history.size() - 1;
            my_iter != history.end() && op_iter != op_history.end();
            ++my_iter, ++op_iter, shift -= 2
        ) {
            if (*my_iter == decision::defect)
                code |= 1 << shift;
            if (*op_iter == decision::cooperate)
                code |= 1 << (shift - 1);
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
