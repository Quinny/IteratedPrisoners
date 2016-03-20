#include "ipd/config.h"
#include "ipd/genetic.h"
#include "common.h"

int main(int argc, char* argv[]) {
    ipd::config::load_config();
    ipd::config::load_cmd_args(argc, argv);

    std::cout << "<fitness function> <average score>" << std::endl;

    auto avg1 = average_using(ipd::genetic::evaluate_vs_tft, 30);
    std::cout << "tft " << avg1 << std::endl;

    auto avg2 = average_using(ipd::genetic::evaluate_async, 30);
    std::cout << "multi " << avg2 << std::endl;

    auto avg3 = average_using(ipd::genetic::evaluate_tft_dist, 30);
    std::cout << "hamming dist " << avg3 << std::endl;
}
