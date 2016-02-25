#ifndef GENETIC_H
#define GENETIC_H

#include <cmath>
#include <random>
#include <ctime>
#include <functional>

#include "common.h"
#include "bots.h"

using namespace std::placeholders;

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

template <typename T>
T random_range(T first, T last) {
    static std::knuth_b gen (std::time(nullptr));
    std::uniform_int_distribution<T> dist(first, last);
    return dist(gen);
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

    // Encodes the moves of this player and the opponent player into
    // an integer which represents the strategy index
    //
    // Moves are encoded such that a cooperate results in a 1 bit,
    // and a defect results in a 0.  The bits are aligned so that
    // the LEAST significant bit represents the latest move.
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
    auto f = [] (std::string s, decision d) {
        if (d == decision::cooperate) s += 'c';
        else s += 'd';
        return s;
    };
    auto name = std::accumulate(gs.strategy.begin(), gs.strategy.end(),
            std::string(), f);
    return {name, gs};
}

// returns a weighted random sample of v.  The weight of each element
// is represented by the int in the pair.  It is based on the following
// idea:
//      - Generate a random integer P <- [0, sum(weights)]
//      - iterate through the vector, and substract each of the weights from P
//      - Once P becomes less than 0, return the last seen element
//
// Modifications have been made to make this algorithm more efficient.
// Since the weights are fixed, we can precompute the cummulative sums, and binary
// search for the value that will cause P to become less than 0
template <typename T>
std::vector<T>
weighted_random_sample(const std::vector<std::pair<T, int>>& v, int n) {
    std::vector<int> cummulative;
    int total = 0;
    for (auto& i: v) {
        total += i.second;
        cummulative.push_back(total);
    }

    std::vector<T> sample;
    for (int i = 0; i < n; ++i) {
        int prob = random_range(0, total);
        auto it = std::upper_bound(cummulative.begin(), cummulative.end(), prob);
        auto index = it - cummulative.begin();
        sample.push_back(v[index].first);
    }

    return sample;
}

// Cross two genome strings, maybe be differing lengths
std::pair<std::string, std::string> cross(const std::string& s1,
                                          const std::string& s2) {
    auto point = random_range(0UL, std::min(s1.size(), s2.size()));
    std::string p1 = s1.substr(0, point),
                p2 = s2.substr(0, point);

    for (auto i = point; i < s1.size(); ++i)
        p2 += s1[i];
    for (auto i = point; i < s2.size(); ++i)
        p1 += s2[i];

    return {p1, p2};
}

std::string mutate(std::string s, const int rate) {
    auto maybe_change = [&] (const char c) {
        if (random_range(0, 100) < rate) {
            auto d = random_decision();
            if (d == decision::defect) return 'd';
            return 'c';
        }
        return c;
    };

    std::transform(s.begin(), s.end(), s.begin(), maybe_change);
    return s;
}

void evolve(int pop_size) {
    // initial population
    std::vector<prisoner_t> population;
    for (int i = 0; i < pop_size; ++i)
        population.push_back(make_prisoner_t(genetic_strategy(3)));

    // fitness
    std::vector<std::pair<prisoner_t, int>> evaluation;
    for (auto i: population)
        evaluation.push_back({i, total_score(i, bots::all, pop_size)});

    // selection weighted on fitness
    auto selection = weighted_random_sample(evaluation, pop_size);

    // cross over
    std::vector<std::string> genomes;
    for (auto i = 0UL; i < selection.size() - 1; ++i) {
        auto p = cross(selection[i].name, selection[i + 1].name);
        genomes.push_back(p.first); genomes.push_back(p.second);
    }

    // mutate
    std::transform(genomes.begin(), genomes.end(), genomes.begin(),
            std::bind(mutate, _1, 30));
}

#endif /* GENETIC_H */
