#include <vector>
#include <functional>
#include <iostream>
#include <list>
#include <iterator>

#include "common.h"
#include "bots.h"
#include "genetic.h"

template <typename T, typename U>
std::ostream& operator << (std::ostream& out, std::pair<T, U> p) {
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

int main() {
    using score_t = std::pair<prisoner_t, int>;
    auto gen_guy  = make_prisoner_t(genetic_strategy(4));
    auto cp       = bots::all;
    cp.push_back(gen_guy);

    auto sb = play_tourny(cp, 100);
    std::copy(
        sb.begin(),
        sb.end(),
        std::ostream_iterator<score_t>(std::cout, "\n")
    );

    evolve(100);

    return 0;
}
