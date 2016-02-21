#ifndef BOTS_H
#define BOTS_H

#include "common.h"

namespace bots {

decision all_coop(history_t _) {
    (void)_;
    return decision::cooperate;
}

decision all_defect(history_t _) {
    (void)_;
    return decision::defect;
}

const std::vector<prisoner_t> all = {
    {"All cooperate", all_coop},
    {"All defect",    all_defect}
};

}
#endif /* BOTS_H */
