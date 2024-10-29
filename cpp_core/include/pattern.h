#ifndef PATTERN_H
#define PATTERN_H

#include "evidence.h"

bool get_from_pattern(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF);

class Pattern {
public:
    virtual bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const { return false; };
};

class OneOnePattern : public Pattern{
public:
    bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const;
};

class TwoRowPattern : public Pattern {
public:
    bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const;
};

class ThreeRowPattern : public Pattern {
protected:
    const int p, q, r;
public:
    ThreeRowPattern(int p, int q, int r): p(p), q(q), r(r) {}
    bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const;
};

class TwoOnePattern : public Pattern {
public:
    bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const;
};

class RectanglePattern : public Pattern {
public:
    bool check(const Board* board, Proof** proof, bool is_required, Proof* NULL_PROOF) const;
};
#endif