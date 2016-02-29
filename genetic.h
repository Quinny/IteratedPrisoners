#ifndef GENETIC_H
#define GENETIC_H

#include <cmath>
#include <random>
#include <ctime>
#include <functional>
#include <future>

#include "common.h"
#include "bots.h"
#include "profile.h"

namespace ipd {

namespace genetic {

using namespace std::placeholders;

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
        strategy(genome.size()),
        mem_size(std::log(genome.size()) / std::log(4))
    {
        // converts a character to a decision
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

std::string tft_genome(int mem_size) {
    int limit = std::pow(mem_size, 4);
    std::string genome = "c";
    for (int i = 1; i < limit; ++i) {
        if (i & 2 > 0) genome += "c";
        genome += "d";
    }
    return genome;
}

// Create a prisoner_t given a genetic strategy, where the name is a string
// representing it's strategy
prisoner_t make_prisoner_t(const genetic_strategy& gs) {
    // converts a decision to a character
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
    qp::profiler selection_prof("weighted sample");
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

// Generate an initial population of size n with memory
// sizes ranging from [first, last]
std::vector<prisoner_t> initial_population(std::size_t n,
        std::size_t first, std::size_t last) {
    std::vector<prisoner_t> ret;
    for (auto i = 0UL; i < n; ++i)
        ret.push_back(make_prisoner_t(genetic_strategy(random_range(first, last))));
    return ret;
}

std::vector<score_t> evaluate_async(const std::vector<prisoner_t>& v) {
    qp::profiler fit_prof("fitness evaluation");
    std::vector<std::future<int>> futs;
    std::vector<score_t> ret(v.size());
    auto fitness = std::bind(total_score, _1, bots::all, 64);

    for (const auto& i: v) {
        futs.emplace_back(std::async(fitness, i));
    }

    for (auto i = 0UL; i < v.size(); ++i) {
        ret.emplace_back(v[i], futs[i].get());
    }

    return ret;
}

std::string mutate_strategy(std::string s, const int rate) {
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

// Cross two genome strings, maybe be differing lengths
std::pair<std::string, std::string> cross(const std::string& s1,
                                          const std::string& s2) {
    auto point = random_range<std::size_t>(0UL, std::min(s1.size(), s2.size()));
    std::string p1 = s1.substr(0, point),
                p2 = s2.substr(0, point);

    for (auto i = point; i < s1.size(); ++i)
        p2 += s1[i];
    for (auto i = point; i < s2.size(); ++i)
        p1 += s2[i];

    return {p1, p2};
}

prisoner_t evolve(int pop_size, int mutation_rate, int generations) {
    auto population = initial_population(pop_size, 3, 3);
    auto mutate     = std::bind(mutate_strategy, _1, mutation_rate);

    for (int i = 0; i < generations; ++i) {
        // fitness
        auto evaluation = evaluate_async(population);

        // selection weighted on fitness
        auto selection = weighted_random_sample(evaluation, pop_size);

        // cross over
        std::vector<std::string> genomes;
        for (auto i = 0UL; i < selection.size() - 1; ++i) {
            auto p = cross(selection[i].name, selection[i + 1].name);
            genomes.push_back(p.first); genomes.push_back(p.second);
        }

        // mutate
        std::transform(genomes.begin(), genomes.end(), genomes.begin(), mutate);

        // build next population
        population.clear();
        for (auto i: genomes)
            population.push_back(make_prisoner_t(genetic_strategy(i)));
    }

    prisoner_t best = population.front();
    int best_score = total_score(best, bots::all, 100);
    for (auto i = 1UL; i < population.size(); ++i) {
        auto score = total_score(population[i], bots::all, 100);
        if (score > best_score) {
            best = population[i];
            best_score = score;
        }
    }

    return best;
}

} // genetic namespace

} // ipd namespace

#endif /* GENETIC_H */
