#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "ipd/common.h"
#include "ipd/bots.h"
#include "ipd/genetic.h"
#include "ipd/profile.h"
#include "ipd/config.h"
#include "ipd/hill_climb.h"

#include "common.h"

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);
    auto set_gensize = ipd::config::setter_for(ipd::config::generations);

    std::cout << "<generation size> <average score>" << std::endl;

    for (int i = 2; i < 1000; i += 50) {
        set_gensize(i);
        auto avg = average_using(ipd::genetic::evaluate_vs_tft, 10);
        std::cout << ipd::config::pop_size << " " << avg << std::endl;
    }
    return 0;
}
