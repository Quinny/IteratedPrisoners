#include <vector>
#include <functional>
#include <iostream>
#include <list>

#include "common.h"
#include "bots.h"

template <typename T, typename U>
std::ostream& operator << (std::ostream& out, std::pair<T, U> p) {
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

int main() {
    prisoner_t allc = {"all cooperate", bots::all_coop};
    prisoner_t alld = {"all defect", bots::all_defect};

    auto score = play(allc, alld, 7);

    std::cout << score << std::endl;
    return 0;
}
