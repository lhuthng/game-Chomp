#include "board.h"
#include <sstream>
#include <utility>
#include <functional>

using namespace std;

bool PairListEqual::operator()(const list<pair<int, int>> *lhs, const list<pair<int, int>> *rhs ) const {
    return lhs->size() == rhs->size() && equal(lhs->begin(), lhs->end(), rhs->begin());
}

// size_t PairListHash::operator()(const list<pair<int, int>>* list) const {
//     size_t hashVal = 0;
//     for (const auto& p : *list) {
//         hashVal ^= hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
//     }
//         return hashVal;
// }
size_t PairListHash::operator()(const list<pair<int, int>> *list) const {
    size_t value = 0;
    hash<int> haser;
    for (const auto& p : *list) {
        value ^= haser(p.first) + 0x9e3779b9 + (value << 6) + (value >> 2);
        value ^= haser(p.second) + 0x9e3779b9 + (value << 6) + (value >> 2);
    }
    return value;
}

const list<pair<int, int>>& Board::get_generators() const {
    return *p_generators;
}

ostream& operator<<(ostream& os, const pair<int, int>& pair) {
    os << "(" << pair.first << "," << pair.second << ")";
    return os;
}

ostream& operator<<(ostream& os, const list<pair<int, int>>& list) {
    os << "<";
    for (const auto & it : list) {
        os << it << ",";
    }
    os << "\b>";
    return os;
}

ostream& operator<<(ostream& os, const Board& board) {
    os << *(board.p_generators);
    return os;
}

bool Board::operator<(const Board& other) const {
    for (auto it = p_generators->begin(), end = prev(p_generators->end()), it2 = other.p_generators->begin(), end2 = other.p_generators->end(); it != end;) {
        const pair spike = {it -> first - 1, next(it)-> second - 1};
        while (it2 != end2 && it2->first > spike.first) it2 = next(it2);
        if (it2 == end2) return true;
        if (it2->second <= spike.second) return false;
        it = next(it);
    }
    return true;
}

template <typename GenIter1, typename GenIter2, typename GetFunc>
pair<int, int> Board::check(GenIter1 begin1, GenIter1 end1, GenIter2 begin2, GenIter2 end2, GetFunc& first, GetFunc& second) {
    auto match = [&first, &second](GenIter1& it1, GenIter2& it2) {return first(*it1) == it2->first && second(*it1) == it2->second;};
    end1 = prev(end1);
    end2 = prev(end2);
    GenIter1 it1 = begin1, rit1 = end1;
    GenIter2 it2 = begin2, rit2 = end2;
    while (it1 != end1 && it2 != end2 && match(it1, it2)) {
        it1 = next(it1);
        it2 = next(it2);
    }
    while (rit1 != begin1 && rit2 != begin2 && match(rit1, rit2)) {
        rit1 = prev(rit1);
        rit2 = prev(rit2);
    }
    if (it1 != rit1) return {-1, -1};
    if (it2 == rit2) {
        if (it2 == begin2 && it1 == begin1) {
            if (first(*it1) < it2->first) return *it1;
            else return {-1, -1};
        } 
        if (it2 == end2 && it1 == end1) {
            if (second(*it1) < it2->second) return *it1;
            else return {-1, -1};
        }
        if (first(*it1) <= rit2->first && second(*it1) <= rit2->second 
            && first(*it1) <= it2->first && second(*it1) <= it2->second)  return *it1;
        return {-1, -1};
    }
    if (it2 != begin2 && prev(it2) == rit2) return *it1;
    if (it2 != end2 && next(it2) != rit2) return {-1, -1};
    if (first(*it1) <= rit2->first && second(*it1) <= rit2->second 
        && first(*it1) <= it2->first && second(*it1) <= it2->second) return *it1;
    return {-1, -1};
}

int get_first(const pair<int, int>& pair) {
    return pair.first;
}

int get_second(const pair<int, int>& pair) {
    return pair.second;
}

pair<int, int> Board::can_reach(const Board* target) const {
    if (this == target) return {-1, -1};
    pair<int, int> result;
    if ((result = check(target->p_generators->begin(), target->p_generators->end(), p_generators->begin(), p_generators->end(), get_first, get_second)) != make_pair(-1, -1)) {
        return result;
    }
    if ((result = check(target->p_generators->begin(), target->p_generators->end(), p_generators->rbegin(), p_generators->rend(), get_second, get_first)) != make_pair(-1, -1)) {
        return {result.second, result.first};
    }
    return {-1, -1};
}

bool Board::contains(pair<int, int> cell) const {
    for (auto &[a, b] : *p_generators) {
        if (cell.first >= a && cell.second >= b) return false;
    }
    return true;
}

// const Board* Board::chomp(const pair<int, int> &position, bool* is_flipped) const {
//     list<pair<int, int>> *generators = new list(*p_generators);
//     auto it = generators->begin();
//     while (it != generators->end()) {
//         if (position.first < it->first) ++it;
//         else if (position.first == it->first) {
//             if (position.second >= it->second) return this;
//             generators->insert(next(it), position);
//             it = next(it);
//             break;
//         }
//         else {
//             generators->insert(it, position);
//             it = prev(it);
//             break;
//         }
//     }
//     while (it != generators->begin() && it->first <= prev(it)->first && it->second <= prev(it)->second) {
//         generators->erase(prev(it));
//     }
//     return get_board(generators, is_flipped, true);
// }

const Board* Board::chomp(const pair<int, int> &position, bool* is_flipped) const {
    list<pair<int, int>> *generators = new list(*p_generators);
    auto it = generators->begin();
    while (it != generators->end()) {
        if (position.first < it->first) ++it;
        else if (position.first == it->first) {
            if (position.second >= it->second) return this;
            generators->insert(next(it), position);
            it = next(it);
            break;
        }
        else {
            generators->insert(it, position);
            break;
        }
    }
    while (it != generators->begin() && it->first <= prev(it)->first && it->second <= prev(it)->second) {
        generators->erase(prev(it));
    }
    return get_board(generators, is_flipped, true);
}

const Board* Board::get_board(int width, int height, bool* is_flipped) {
    list<pair<int, int>> *generators = new list<pair<int, int>>({{width, 0}, {0, height}});
    return get_board(generators, is_flipped);
}

const Board* Board::get_board(string input, bool* is_flipped) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
    if (input.length() < 2 || input.front() != '<' || input.back() != '>' ) {
        throw std::invalid_argument("1. Invalid format " + input);
    }
    input = input.substr(1, input.size() - 2);
    stringstream ss(input);
    string raw_pair;
    list<pair<int, int>> *pairs = new list<pair<int, int>>();
    bool first = true;
    string pair;
    while (getline(ss, raw_pair, ',')) {
        if (first) {
            pair = raw_pair;
        }
        else {
            pair += "," + raw_pair;
            if (pair.length() < 2 || pair.front() != '(' || pair.back() != ')') {
                throw std::invalid_argument("2. Invalid format " + input);
            }
            const size_t comma = pair.find(',');
            int x = stoi(pair.substr(1, comma - 1));
            int y = stoi(pair.substr(comma + 1, pair.size() - comma - 2));
            pairs->emplace_back(x, y);
        }
        first ^= true;
    }
    return get_board(pairs, is_flipped);
}

const Board* Board::get_board(list<pair<int, int>> *generators, bool* is_flipped, bool skip_sorrting) {
    simplify(*generators, is_flipped, skip_sorrting);
    auto it = boards.find(generators);
    if (it != boards.end()) {
        return it->second;
    }
    boards.insert({generators, nullptr});
    it = boards.find(generators);
    it->second = new Board(it->first);
    return it->second;
}

void Board::clean_up() {
    for (auto&[fst, snd] : boards) {
        delete fst;
        delete snd;
    }
    boards.clear();
}

void Board::flip(list<pair<int, int>>& list) {
    if (list.size() <= 1) return;
    for (auto left = list.begin(), right = prev(list.end()); left != next(right); ++left, --right) {
        swap(left->first, right->second);
        if (left != right) swap(left->second, right->first);
        if (left == right || left == prev(right)) break;
    }
}

void Board::simplify(list<pair<int, int>>& generators, bool* is_flipped, bool skip_sorting) {
    if (generators.empty()) generators.emplace_back(0, 0);
    // if (!skip_sorting) generators.sort([](const pair<int, int>& lhs, const pair<int, int>& rhs) {
    //     return lhs.first > rhs.first || lhs.first == rhs.first && lhs.second < rhs.second;
    // });
    // for (auto it = generators.begin(); it != prev(generators.end());) {
    //     if (auto nit = next(it); 
    //         it->first > nit->first 
    //         && it->second < nit->second || it->first == nit->first 
    //         && it->second < nit->second
    //     ) it++;
    //     else if (it->first > nit->first && it->second >= nit->second) {
    //         generators.erase(it);
    //         if (nit != generators.begin()) it = prev(nit);
    //         else it = nit;
    //     }
    //     else generators.erase(nit);
    // }
    if (!skip_sorting) generators.sort([](const pair<int, int>& lhs, const pair<int, int>& rhs) {
        return lhs.first > rhs.first || lhs.first == rhs.first && lhs.second > rhs.second;
    });
    for (auto it = generators.begin(); it != prev(generators.end());) {
        auto nit = next(it);
        if (it->first >= nit->first && it->second >= nit->second) {
            generators.erase(it);
            if (nit != generators.begin()) it = prev(nit);
            else it = nit;
        }
        else it++;
    }
    if (is_flipped != nullptr) *is_flipped = false;
    for (auto left = generators.begin(), right = prev(generators.end()); left != generators.end(); ++left, --right) {
        if (left->first == right->second) continue;
        if (left->first < right->second) {
            flip(generators); 
            if (is_flipped != nullptr) *is_flipped = true;
        }
        break;
    }
    generators.begin()->second = 0;
    prev(generators.end())->first = 0;
}

unordered_map<list<std::pair<int, int>>*, Board*, PairListHash, PairListEqual> Board::boards;
const Board* Board::EMPTY = Board::get_board(0, 0);
const Board* Board::POISON = Board::get_board(1, 1);