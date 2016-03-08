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
        auto score = ipd::total_score(guy, ipd::bots::all, ipd::config::rounds);
        total += score;
        log(guy, score);
    }
    return total / n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    std::cout << "average over 100 rounds using "
        << "pop size = " << ipd::config::pop_size << " and generations = "
        << ipd::config::generations << " is ";

    auto avg = average_using(ipd::genetic::evaluate_vs_tft, 100);
    std::cout << avg << std::endl;
}
