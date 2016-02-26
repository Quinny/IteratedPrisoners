#include <iostream>
#include <fstream>
#include <chrono>

#include "common.h"
#include "bots.h"
#include "genetic.h"

// Run the function f(args) and return the execution time in seconds
template <typename Func, typename... Args>
double time_operation(Func&& f, Args&&... args) {
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    f(args...);
    auto t2 = high_resolution_clock::now();
    return duration_cast<duration<double>>(t2 - t1).count();
}

int main() {
    auto t = time_operation([] () {
        using namespace ipd;

        auto ev_guy   = genetic::evolve(100, 30, 10);
        auto cp       = bots::all;

        cp.push_back(ev_guy);

        auto sb = play_tourny(cp, 100);
        std::sort(sb.begin(), sb.end(), [] (score_t x, score_t y) {
                return x.second > y.second;
        });

        for (auto i: sb) {
            std::cout << i.first << ", " << i.second << std::endl;
        }

        // append winner to a text file for later analysis
        std::fstream f("winners.txt", std::ios_base::app);
        f << sb.front().first << " " << sb.front().second << std::endl;
    });

    std::cout << "ran in " << t << std::endl;
    return 0;
}
