#ifndef EVIDENCE_H
#define EVIDENCE_H

#include "board.h"
#include "iterator.h"
#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>

struct Proof {
    const std::pair<int, int> move;
    Proof(const std::pair<int, int>& move) : move(move) {}
};

struct BoardEqual {
    bool operator()(const Board* lsh, const Board* rhs) const;
};

struct BoardHash {
    size_t operator()(const Board* board) const;
};

class Evidence {
protected:
    IteratorCode code;
    std::unordered_map<const Board*, Proof*, BoardHash, BoardEqual> memory;
public:
    Evidence(IteratorCode code);
    virtual Proof* get(const Board* board);
    ~Evidence();
};

class ExtendedEvidence : public Evidence {
private:
    Proof* NULL_PROOF;
    Proof* get(const Board* board, bool is_required);
    bool get_from_P_positions(const Board* board, Proof** proof);
    std::set<const Board*> P_positions;
public:
    ExtendedEvidence(IteratorCode code);
    Proof* get(const Board* board);
    ~ExtendedEvidence();
    int P_size() { return P_positions.size(); }
};

#endif