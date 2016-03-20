#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>

#include "ipd/common.h"
#include "ipd/genetic.h"
#include "ipd/config.h"

#include "common.h"

void set_memsize(int n) {
    using namespace ipd::config;
    mem_first = mem_last = n;
}

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    std::cout << "<memory size> <average score>" << std::endl;

    for (int i = 1; i <= 5; ++i) {
        set_memsize(i);
        auto avg = average_using(ipd::genetic::evaluate_vs_tft, 50);
        std::cout << ipd::config::mem_first << " " << avg << std::endl;
    }

    return 0;
}
