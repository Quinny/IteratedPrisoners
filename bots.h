#ifndef BOTS_H
#define BOTS_H

#include <algorithm>

#include "common.h"

// All default stateless bots

namespace bots {

// Always cooperate
decision all_coop(const history_t& _) {
    (void)_;
    return decision::cooperate;
}

// Always defect
decision all_defect(const history_t& _) {
    (void)_;
    return decision::defect;
}

// Start by cooperating, and then mirror opponents last move
decision tft(const history_t& h) {
    if (h.empty()) return decision::cooperate;
    return h.front();
}

// Start by defecting and then mirror opponents last move
decision sus_tft(const history_t& h) {
    if (h.empty()) return decision::defect;
    return h.front();
}

// Start by cooperating, and then only defect if the opponent has defected
// twice in a row
decision tf2t(const history_t& h) {
    if (h.size() < 2)
        return decision::cooperate;
    if (h.front() == decision::defect && *std::next(h.begin()) == decision::defect)
        return decision::defect;
    return decision::cooperate;
}

// Start by defecting, and then only defect if the opponent has defected
// twice in a row
decision sus_tf2t(const history_t& h) {
    if (h.size() < 2)
        return decision::defect;
    if (h.front() == decision::defect && *std::next(h.begin()) == decision::defect)
        return decision::defect;
    return decision::cooperate;
}

// Cooperate until the opponent defects, then continually
// defect
decision grudger(const history_t& h) {
    if (std::find(h.begin(), h.end(), decision::defect) != h.end())
        return decision::defect;
    return decision::cooperate;
}

// Defect until the opponent cooperates, then continually cooperate
decision sucker(const history_t& h) {
    if (std::find(h.begin(), h.end(), decision::cooperate) != h.end())
        return decision::cooperate;
    return decision::defect;
}

// Start by defecting, and then only cooperate if the opponent has
// cooperated twice in a row
decision hesitant(const history_t& h) {
    if (h.size() < 2)
        return decision::defect;
    if (h.front() == decision::cooperate &&
            *std::next(h.begin()) == decision::cooperate)
        return decision::cooperate;
    return decision::defect;
}

// Vector of bots converted to prisoners
const std::vector<prisoner_t> all = {
    {"All cooperate",     all_coop},
    {"All defect",        all_defect},
    {"Tit for tat",       tft},
    {"Sus Tit for tat",   sus_tft},
    {"Tit for 2 tat",     tf2t},
    {"Grudger",           grudger},
    {"Sucker",            sucker},
    {"Sus Tit for 2 tat", sus_tf2t},
    {"Hesitant",          hesitant}
};

}
#endif /* BOTS_H */
