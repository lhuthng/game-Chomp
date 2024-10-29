#ifndef ITERATOR_H
#define ITERATOR_H

#include <list>
#include "board.h"

class AbstractIterator {
protected:
    const Board* board;
    std::pair<int, int> current, end;
    virtual AbstractIterator& advance() = 0;
public: 
    AbstractIterator(const Board* board);
    bool hasNext() const;
    std::pair<int, int> getNext();
    virtual ~AbstractIterator() {}
};

enum class IteratorCode {
    NORMAL,
//  REVERSED,
    ZIGZAG
};

class IteratorFactory {
public:
    static AbstractIterator* create(IteratorCode code, const Board* board);
};

class Iterator : public AbstractIterator {
private:
    std::list<std::pair<int, int>>::const_iterator prev, it;
protected:
    AbstractIterator& advance();
public:
    Iterator(const Board* board);
    virtual void init();
};

class ZigZagIterator : public AbstractIterator {
private:
    int limit;
protected:
    AbstractIterator& advance();
public:
    ZigZagIterator(const Board* board);
    virtual void init();
};

#endif