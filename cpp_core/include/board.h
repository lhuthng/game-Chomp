#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>

struct PairListEqual {
    bool operator()(const std::list<std::pair<int, int>> *lhs, const std::list<std::pair<int, int>> *rhs) const;
};

struct PairListHash {
    size_t operator()(const std::list<std::pair<int, int>> *list) const;
};

class Board {
private:
    const std::list<std::pair<int, int>>* p_generators;
    Board(const std::list<std::pair<int, int>> * p_generators) { this->p_generators = p_generators; }

    static std::unordered_map<std::list<std::pair<int, int>>*, Board*, PairListHash, PairListEqual> boards;
    static void flip(std::list<std::pair<int, int>>& list);
    static void simplify(std::list<std::pair<int, int>>& generators, bool* is_flipped=nullptr, bool skip_sorting=false);
public:
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    const std::list<std::pair<int, int>>& get_generators() const;
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    bool operator<(const Board& other) const;
    template <typename GenIter1, typename GenIter2, typename GetFunc>
    static std::pair<int, int> check(GenIter1 begin1, GenIter1 end1, GenIter2 begin2, GenIter2 end2, GetFunc& first, GetFunc& second);
    std::pair<int, int> can_reach(const Board* target) const;
    bool contains(std::pair<int, int> cell) const;
    const Board* chomp(const std::pair<int, int> &position, bool* is_flipped=nullptr) const;

    static int total_size() { return boards.size(); }
    static const Board* get_board(int width, int height, bool* is_flipped=nullptr);
    static const Board* get_board(std::string input, bool* is_flipped=nullptr);
    static const Board* get_board(std::list<std::pair<int, int>> *generators, bool* is_flipped=nullptr, bool skip_sorting=false);
    static void clean_up();
    static const Board *POISON, *EMPTY;
};

#endif