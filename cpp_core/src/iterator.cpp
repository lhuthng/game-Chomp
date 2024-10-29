#include "iterator.h"
#include <iostream>

using namespace std;


bool AbstractIterator::hasNext() const {
    return current != end;
}

pair<int, int> AbstractIterator::getNext() {
    pair<int, int> result = current;
    advance();
    return result;
}

AbstractIterator::AbstractIterator(const Board* board) {
    this->board = board;
}

Iterator::Iterator(const Board* board) : AbstractIterator(board) {
    end = {0, std::prev(board->get_generators().end())->second};
    init();
}

AbstractIterator& Iterator::advance() {
    if (it != board->get_generators().end()) {
        if (current.first < prev->first - 1) {
            current.first += 1;
        }
        else {
            current.first = 0;
            current.second += 1;
            if (current.second > it->second - 1) prev = it++;
        }
    }
    return *this;
}

AbstractIterator* IteratorFactory::create(IteratorCode code, const Board* board) {
    switch (code) {
        case IteratorCode::NORMAL: return new Iterator(board);
        case IteratorCode::ZIGZAG: return new ZigZagIterator(board);
    }
    return nullptr;
};

void Iterator::init() {
    it = this->board->get_generators().begin();
    prev = it++;
    if (it == board->get_generators().end()) {
        current = {0, 0};
    }
    else {
        current = {1, 0};
        if (prev->first <= 1) current = {0, 1};
        if (end == make_pair(0, 0)) current = end;
    }
}

ZigZagIterator::ZigZagIterator(const Board* board) : AbstractIterator(board) {
    limit = 1;
    end = {-1, 1};
    const list<pair<int, int>>& generator = board->get_generators();
    for (auto it = generator.begin(), nxt = next(it); nxt != generator.end(); it++, nxt++) {
        int peak = it->first + nxt->second - 1;
        if (limit <= peak) {
            if (limit < peak) limit = peak;
            end = { it->first - 2, nxt->second };
            if (end.first == -1) {
                end.first += end.second + 1;
                end.second = 0;
            }
        }
    }
    init();
}

AbstractIterator& ZigZagIterator::advance() {
    
    do {
        current.first -= 1;
        current.second += 1;
        if (current.first == -1) {
            current.first += current.second + 1;
            current.second = 0;
        }
    } while (current != end && !board->contains(current));
    return *this;
}

void ZigZagIterator::init() {
    if (end == make_pair(-1, 1) || end == make_pair(0, 0)) current = end;
    else current = {1, 0};
}