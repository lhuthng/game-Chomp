 #include "pattern.h"

using namespace std;

bool OneOnePattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() >= 2) {
        auto first = generators.begin(), last = prev(generators.end());
        if (first->first == last->second) {
            if (!board->contains({1, 1})) {
                *proof = nullptr;
                return true;
            }
            *proof = new Proof({1, 1});
            return true;
        }
        else if (generators.size() == 3 && *next(first) == make_pair(1, 1)) {
            if (first->first > last->second) 
                *proof = new Proof({last->second, 0});
            else 
                *proof = new Proof({0, first->first});
            return true;
        }
    }
    return false;
}

bool TwoRowPattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (prev(generators.end())->second != 2 || generators.size() != 3) return false;
    auto it = generators.begin();
    const int first = (it++)->first, second = (it++)->first;
    if (first - second == 1) {
        *proof = nullptr;
        return true;
    }
    return false;
}

bool ThreeRowPattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    if (generators.size() == 4 && prev(generators.end())->second == 3) {
        auto it = generators.begin();
        const int first = (it++)->first, second = (it++)->first, third = (it++)->first;
        if (third == r && second >= q && first - second == p - q) {
            *proof = nullptr;
            return true;
        }
    }
    else if (prev(generators.end())->second == 2) {
        auto first = generators.begin(), second = next(first);
        if (second->second == 2) {
            *proof = new Proof({first->first - 1, 1});
            return true;
        }
        else if (second->first < first->first - 1) {
            *proof = new Proof({second->first + 1, 0});
            return true;
        }
    }
    return false;
}

int get_mex(int p, int q=0) {
    if (q == 0) return p - 1;
    if ((p - q) % 2 == 0) return p + (q - 1) / 2;
    if (2*q <= p) return p - (q + 1) / 2 - 1;
    return 3 * (p - q - 1) / 2;
}

bool TwoOnePattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    auto& generators = board->get_generators();
    int p1, p2, q2;
    bool flip = false;
    if (!board->contains({2, 1}) && generators.size() >= 2) {
        p1 = generators.begin()->first - 1;
        p2 = prev(generators.end())->second - 1;
        q2 = p2;
        if (prev(prev(generators.end()))->first == 1) {
            q2 = prev(prev(generators.end()))->second - 1;
        }
        flip = false;
    }
    else if (!board->contains({1, 2}) && generators.size() >= 2) {
        p1 = prev(generators.end())->second - 1;
        p2 = generators.begin()->first - 1;
        q2 = p2;
        if (next(generators.begin())->second == 1) {
            q2 = next(generators.begin())->first - 1;
        }
        flip = true;
    }
    else goto SKIP;
    {
        int mex1 = get_mex(p1), mex2 = get_mex(p2, q2);
        if (mex1 == mex2) {
            *proof = nullptr;
            return true;
        }
        else {
            if (mex1 > mex2) {
                pair move = flip ? make_pair(0, 2 + mex2) : make_pair(2 + mex2, 0);
                *proof = new Proof(move);
                return true;
            }
            for (int q = 0; q < q2; q++) {
                if (get_mex(p2, q) == mex1) {
                    pair move = flip ? make_pair(q + 1, 1) : make_pair(1, q + 1);
                    *proof = new Proof(move);
                    return true;
                }
            }
            for (int p = 1; p < p2; p++) {
                int qq = min(p, q2);
                if (get_mex(p, qq) == mex1) {
                    pair move = flip ? make_pair(p + 1, 0) : make_pair(0, p + 1);
                    *proof = new Proof(move);
                    return true;
                }
            }
        }
    }
SKIP:
    return false;
}

bool RectanglePattern::check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const {
    if (!is_required && board->get_generators().size() == 2) {
        *proof = NULL_PROOF;
        return true;
    }
    return false;
}

const list<Pattern*> patterns({
    new OneOnePattern(),
    new TwoOnePattern(),
    new TwoRowPattern(),
    new ThreeRowPattern(4, 2, 2),
    new ThreeRowPattern(11, 7, 5),
    new ThreeRowPattern(15, 10, 7),
    new ThreeRowPattern(18, 12, 9),
    new ThreeRowPattern(26, 19, 11),
    new ThreeRowPattern(30, 21, 14),
    new ThreeRowPattern(35, 24, 17),

    new RectanglePattern(),
});

bool get_from_pattern(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) {
    auto generators = board->get_generators();
    if (board == Board::EMPTY) {
        *proof = new Proof({0, 0});
        return true;
    }
    else if (board == Board::POISON) {
        *proof = nullptr;
        return true;
    }
    else if (generators.size() == 2) {
        const pair<int, int>& first = *generators.begin();
        const pair<int, int>& second = *next(generators.begin());
        if (second == make_pair(0, 1)) {
            *proof = new Proof({1, 0});
            return true;
        }
    }
    for (const Pattern* pattern: patterns) {
        if (pattern->check(board, proof, is_required, NULL_PROOF)) return true;
    }
    return false;
}
