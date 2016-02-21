#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <list>
#include <functional>
#include <vector>

enum class decision {
    cooperate,
    defect
};

using history_t  = std::list<decision>;
using strategy_t = std::function<decision(history_t)>;

struct prisoner_t {
    std::string name;
    strategy_t strategy;

    decision operator () (history_t h) {
        return strategy(h);
    }
};


// score for player that made d1
// TODO
// make this 4 if statements, this is ugly
int score(decision d1, decision d2) {
    if (d1 == decision::defect) {
        if (d2 == decision::defect)
            return 1;
        else
            return 5;
    }

    else {
        if (d2 == decision::defect)
            return 0;
        else
            return 3;
    }
    return 0;
}

int play(prisoner_t p1, prisoner_t p2, int n) {
    history_t p1_hist, p2_hist;
    int p1_score = 0;

    for (int i = 0; i < n; ++i) {
        auto d1 = p1(p2_hist);
        auto d2 = p2(p1_hist);
        p1_score += score(d1, d2);

        p1_hist.push_front(d1);
        p2_hist.push_front(d2);
    }
    return p1_score;
}

// This could be used as a fitness function
int total_score(prisoner_t p, std::vector<prisoner_t> ps, int n) {
    int score = 0;
    for (auto i: ps)
        score += play(p, i, n);
    return score;
}

std::vector<std::pair<prisoner_t, int>>
play_tourny(std::vector<prisoner_t> players, int n) {
    std::vector<std::pair<prisoner_t, int>> ret;
    for (auto p1: players) {
        auto score = total_score(p1, players, n);
        ret.push_back({p1, score});
    }
    return ret;
}

#endif /* COMMON_H */
