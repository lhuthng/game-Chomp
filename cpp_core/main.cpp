#include <iostream>
#include "board.h"
#include "iterator.h"
#include "evidence.h"
#include <chrono>
#include <sstream>

using namespace std;


void print(const pair<int, int>& pair) {
    cout << "(" << pair.first << "," << pair.second << ")";
}

int main() {
    const Board* board = Board::get_board("<(12,0),(0, 9)>");
    ExtendedEvidence e(IteratorCode::ZIGZAG);
    auto start = std::chrono::high_resolution_clock::now();
    Proof* proof = e.get(board);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " milliseconds" << std::endl;
    if (proof != nullptr) {
        cout << *board << " " << proof->move.first << " " << proof->move.second << endl;
    }
    Board::clean_up();
}