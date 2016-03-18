#include "ipd/config.h"
#include "ipd/genetic.h"

void log(const ipd::prisoner_t& p, int score) {
    std::fstream f("winners.txt", std::ios_base::app);
    f << p << " " << score << std::endl;
}

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
        log(guy, score);
    }
    return total / n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

/*    std::cout << "average over 100 rounds using "
        << "pop size = " << ipd::config::pop_size << " and generations = "
        << ipd::config::generations << " is ";
*/
    auto avg1 = average_using(ipd::genetic::evaluate_vs_tft, 30);
    std::cout << "tft: " << avg1 << std::endl;

    auto avg2 = average_using(ipd::genetic::evaluate_async, 30);
    std::cout << "multi: " << avg2 << std::endl;

    auto avg3 = average_using(ipd::genetic::evaluate_tft_dist, 30);
    std::cout << "hamm: " << avg3 << std::endl;
}
