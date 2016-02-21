#ifndef BOTS_H
#define BOTS_H

#include <algorithm>

#include "common.h"

// TODO
// bring over all other bots from IPD.hs

namespace bots {

decision all_coop(history_t _) {
    (void)_;
    return decision::cooperate;
}

decision all_defect(history_t _) {
    (void)_;
    return decision::defect;
}

decision tft(history_t h) {
    if (h.empty()) return decision::cooperate;
    return h.front();
}

decision sus_tft(history_t h) {
    if (h.empty()) return decision::defect;
    return h.front();
}

decision tf2t(history_t h) {
    if (h.size() < 2)
        return decision::cooperate;
    if (h.front() == decision::defect && *std::next(h.begin()) == decision::defect)
        return decision::defect;
    return decision::cooperate;
}

decision sus_tf2t(history_t h) {
    if (h.size() < 2)
        return decision::defect;
    if (h.front() == decision::defect && *std::next(h.begin()) == decision::defect)
        return decision::defect;
    return decision::cooperate;
}

decision grudger(history_t h) {
    if (std::find(h.begin(), h.end(), decision::defect) != h.end())
        return decision::defect;
    return decision::cooperate;
}

decision sucker(history_t h) {
    if (std::find(h.begin(), h.end(), decision::cooperate) != h.end())
        return decision::cooperate;
    return decision::defect;
}

decision hesitant(history_t h) {
    if (h.size() < 2)
        return decision::defect;
    if (h.front() == decision::cooperate &&
            *std::next(h.begin()) == decision::cooperate)
        return decision::cooperate;
    return decision::defect;
}

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
