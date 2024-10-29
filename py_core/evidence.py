from .board import *
from .iterator import *

class Evidence():
    def __init__(self, counter_move=False, only_one=False, trace_back_fn=iterator):
        self.memory = dict()
        self.memory[()] = ()
        self.memory[(1,)] = False
        self.only_one=only_one
        self.trace_back_fn = trace_back_fn
        self.counter_move = counter_move

        self.mex = { (1,): 0 }

    def get(self, board, auto=True):
        if board.data in self.memory: return self.memory[board.data]
        found = False
        for move in self.trace_back_fn(board):
            child = board.chomp(*move, auto)
            if self.get(child) == False:
                found = True
                counter = move if self.counter_move else child.data
                if self.only_one:
                    self.memory[board.data] = counter
                    break
                else: 
                    self.memory[board.data] = self.memory.get(board.data, [])
                    self.memory[board.data].append(counter)
        if not found:
            self.memory[board.data] = False
        return self.memory[board.data]
    
    def get_mex(self, board):
        if not isinstance(board, Board): 
            board = Board(board)
        if board.data not in self.mex:
            nimbers = set()
            for move in self.trace_back_fn(board):
                nimbers.add(self.get_mex(board.chomp(*move)))
            m = max(nimbers) if len(nimbers) != 0 else 0
            for i in range(0,m + 2):
                if i not in nimbers:
                    self.mex[board.data] = i
                    break
        return self.mex[board.data]
    
    @staticmethod
    def get_mex_2(p: int, q: int):
        if q % 2 == p % 2:
            return p + (q - 1) // 2
        if 2 * q <= p:
            return p - (q + 1)//2 - 1
        else:
            return 3*((p-q-1) // 2)