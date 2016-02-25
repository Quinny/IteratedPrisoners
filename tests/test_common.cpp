#include <vector>

#include "test.h"
#include "../common.h"
#include "../bots.h"

const decision d = decision::defect;
const decision c = decision::cooperate;

void test_score() {
    test::assert_true(
            score(c, d) == 0
            && score(d, c) == 5
            && score(c, c) == 3
            && score(d, d) == 1,
            "scores correctly"
        );
}

void test_play() {
    test::assert_true(
            play(
                {"", bots::all_coop},
                {"", bots::all_defect},
                10
            ) == 0,
            "all coop vs all defect"
        );

    test::assert_true(
            play(
                {"", bots::all_defect},
                {"", bots::all_coop},
                10
            ) == 50,
            "all defect vs all coop"
        );

    test::assert_true(
            play(
                {"", bots::all_coop},
                {"", bots::all_coop},
                10
            ) == 30,
            "all coop vs all coop"
        );
}

int main() {
    const std::vector<test::test_fn> tests = {
        test_score, test_play
    };

    test::run_all(tests.begin(), tests.end());
    return 0;
}
