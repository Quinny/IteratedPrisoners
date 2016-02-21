#ifndef BOTS_H
#define BOTS_H

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

const std::vector<prisoner_t> all = {
    {"All cooperate", all_coop},
    {"All defect",    all_defect},
    {"Tit for tat",   tft}
};

}
#endif /* BOTS_H */
