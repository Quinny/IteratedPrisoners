#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <list>
#include <functional>
#include <vector>
#include <future>
#include <iostream>
#include <algorithm>

namespace ipd {

// A bot can decide to cooperate or defect
enum class decision {
    cooperate,
    defect
};

// Types used for representing move history, and strategy functions
using history_t  = std::list<decision>;
using strategy_t = std::function<decision(const history_t&)>;

// A prisoner has a name and a strategy
struct prisoner_t {
    std::string name;
    strategy_t strategy;

    decision operator () (const history_t& h) const {
        return strategy(h);
    }
};

using score_t = std::pair<prisoner_t, int>;

std::ostream& operator << (std::ostream& out, const prisoner_t& p) {
    out << p.name;
    return out;
}

std::ostream& operator << (std::ostream& os, const score_t& s) {
    os << "(" << s.first << ", " << s.second << ")";
    return os;
}

// Modified prisoners dilemma score function.  Re arranged so that
// a higher score => less time spent in jail.  This function will return the
// score obtained by the bot who made decision d1.  To get d2's score, the
// arguments should be reversed
int score(decision d1, decision d2) {
    if (d1 == decision::cooperate && d2 == decision::cooperate)
        return 3;
    if (d1 == decision::cooperate && d2 == decision::defect)
        return 0;
    if (d1 == decision::defect && d2 == decision::cooperate)
        return 5;
    if (d1 == decision::defect && d2 == decision::defect)
        return 1;
    return 0;
}

// Play one prisoner against another for n rounds,
// and record prisoner p1's score.  To get p2's score, the arguments
// should be reversed.  Doing it this way makes it so that each
// prisoner gets the opportunity to go first, thus removing bias
int play(const prisoner_t& p1, const prisoner_t& p2, int n) {
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

// Play prisoner p against all others for n rounds and return
// the total score obtained
int total_score(const prisoner_t& p, const std::vector<prisoner_t>& ps, int n) {
    int score = 0;
    for (const auto& i: ps)
        score += play(p, i, n);
    return score;
}

bool sort_compare(const score_t& x, const score_t& y) {
    return x.second > y.second;
}

bool score_compare(const score_t& x, const score_t& y) {
    return x.second < y.second;
}

std::vector<score_t>
play_tourny(const std::vector<prisoner_t>& players, int n) {
    std::vector<std::pair<prisoner_t, int>> ret;
    for (auto p1: players) {
        auto score = total_score(p1, players, n);
        ret.push_back({p1, score});
    }
    std::sort(ret.begin(), ret.end(), sort_compare);
    return ret;
}

}

#endif /* COMMON_H */
