#include <vector>
#include <functional>
#include <iostream>
#include <list>

#include "common.h"
#include "bots.h"

int main() {
    prisoner_t allc = {"all cooperate", bots::all_coop};
    prisoner_t alld = {"all defect", bots::all_defect};

    auto score = play(allc, alld, 7);

    std::cout << score << std::endl;
    return 0;
}
