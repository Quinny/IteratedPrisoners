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
    auto sb = play_tourny(bots::all, 100);

    for (auto i: sb)
        std::cout << i << std::endl;
    return 0;
}
