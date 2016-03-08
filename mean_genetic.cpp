#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

#include "ipd/common.h"
#include "ipd/bots.h"
#include "ipd/genetic.h"

std::vector<std::pair<std::string, int>> of_memsize(int n) {
    std::size_t looking_for = std::pow(4, n);
    std::string genome; int score;
    std::ifstream f("winners.txt");
    std::vector<std::pair<std::string, int>> ret;

    while (f >> genome >> score) {
        if (genome.size() == looking_for)
            ret.emplace_back(genome, score);
    }
    return ret;
}

std::size_t sum_of(std::vector<std::pair<std::string, int>> ps, int i, char c) {
    std::size_t ret = 0;
    for (auto p: ps) {
        if (p.first[i] == c)
            ret += p.second;
    }
    return ret;
}

int main() {
    auto winners = of_memsize(3);

    std::size_t total_sum = 0;
    for (const auto& i: winners)
        total_sum += i.second;

    std::string mean_genome;
    for (unsigned i = 0; i < winners.front().first.size(); ++i) {
        if (sum_of(winners, i, 'c') >= total_sum / 2)
            mean_genome += 'c';
        else
            mean_genome += 'd';
    }
    std::cout << mean_genome << std::endl;
    auto p = ipd::genetic::make_prisoner_t(
        ipd::genetic::genetic_strategy(mean_genome)
    );

    std::cout << ipd::total_score(p, ipd::bots::all, 100) << std::endl;
}
