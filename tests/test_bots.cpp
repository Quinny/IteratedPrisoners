#include "test.h"
#include "../bots.h"

using namespace ipd;

const decision d = decision::defect;
const decision c = decision::cooperate;

void test_coop() {
    test::assert_true(
            bots::all_coop({d, d, c}) == c,
            "always cooperate"
        );
}

void test_defect() {
    test::assert_true(
            bots::all_defect({c, c, c}) == d,
            "always defect"
        );
}

void test_tft() {
    test::assert_true(
            bots::tft({}) == c,
            "cooperate first"
        );

    test::assert_true(
            bots::tft({c, d, c}) == c,
            "mirror opponent coop"
        );

    test::assert_true(
            bots::tft({d, c, c}) == d,
            "mirror opponent defect"
        );
}

void test_sus_tft() {
    test::assert_true(
            bots::sus_tft({}) == d,
            "defect first"
        );

    test::assert_true(
            bots::sus_tft({c, d, c}) == c,
            "mirror opponent coop"
        );

    test::assert_true(
            bots::sus_tft({d, c}) == d,
            "mirror opponent defect"
        );
}

void test_tf2t() {
    test::assert_true(
            bots::tf2t({c}) == c
            && bots::tf2t({d}) == c,
            "cooperate first"
        );

    test::assert_true(
           bots::tf2t({d, d, c}) == d,
           "retaliate on two"
        );

    test::assert_true(
            bots::tf2t({d, c, d}) == c,
            "cooperate otherwise"
        );
}

void test_sus_tf2t() {
    test::assert_true(
            bots::sus_tf2t({c}) == d
            && bots::sus_tf2t({d}) == d,
            "defect first"
        );

    test::assert_true(
           bots::sus_tf2t({d, d, c}) == d,
           "retaliate on two"
        );

    test::assert_true(
            bots::sus_tf2t({d, c, d}) == c,
            "cooperate otherwise"
        );
}

void test_grudger() {
    test::assert_true(
            bots::grudger({c, c, c}) == c,
            "no defects"
        );

    test::assert_true(
            bots::grudger({}) == c,
            "cooperate first"
        );

    test::assert_true(
            bots::grudger({c, c, c, d}) == d,
            "holds grudge"
        );
}

void test_sucker() {
    test::assert_true(
            bots::sucker({d, d, d}) == d,
            "no cooperates"
        );

    test::assert_true(
            bots::sucker({}) == d,
            "defect first"
        );

    test::assert_true(
            bots::sucker({d, d, d, c}) == c,
            "be a sucker"
        );
}

void test_hesitant() {
    test::assert_true(
            bots::hesitant({}) == d,
            "defect first"
        );
    
    test::assert_true(
           bots::hesitant({c, d, c}) == d,
           "hesitate"
        );

    test::assert_true(
            bots::hesitant({c, c, d, d, d}) == c,
            "trust"
        );
}

int main() {
    const std::vector<test::test_fn> tests = {
        test_coop, test_defect, test_tft, test_sus_tft,
        test_tf2t, test_sus_tf2t, test_grudger, test_sucker,
        test_hesitant
    };

    test::run_all(tests.begin(), tests.end());
}
