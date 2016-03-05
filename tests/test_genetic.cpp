#include <algorithm>

#include "test.h"
#include "../ipd/genetic.h"

using namespace ipd;
using namespace ipd::genetic;

const decision d = decision::defect;
const decision c = decision::cooperate;

void test_random() {
    auto rd = random_decision();
    test::assert_true(
            rd == c
            || rd == d,
            "random decision"
        );

    auto x = random_range(10, 100);
    test::assert_true(
            x >= 10 && x <= 100,
            "random range"
        );
}

void test_strategy() {
    genetic_strategy gs(3);
    test::assert_true(
            gs.strategy.size() == 64
            && gs.mem_size == 3,
            "strategy vector size"
        );

    gs = genetic_strategy("ccdd");
    test::assert_true(
            gs.mem_size == 1
            && gs.strategy.size() == 4
            && gs.strategy == std::vector<decision>({c, c, d, d}),
            "strategy from genome"
        );

    gs = genetic_strategy("ccdd");
    gs.history = {c};
    test::assert_true(
            gs.encode({c}) == 3,
            "history encoding"
        );

    test::assert_true(
            gs({c}) == gs.strategy[3],
            "strategy play"
        );

    auto p = make_prisoner_t(gs);
    test::assert_true(
            p.name    == "ccdd"
            && p({c}) == gs({c})
            && p({d}) == gs({d}),
            "strategy to prisoner"
        );
}

int main() {
    const std::vector<test::test_fn> tests = {
        test_random, test_strategy
    };

    test::run_all(tests.begin(), tests.end());
}
