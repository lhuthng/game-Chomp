def trim(board: tuple)->tuple:
    length = len(board)
    while length != 0 and board[length - 1] == 0: 
        length -= 1
    return board[:length]
def flip(board: tuple)->tuple:
    return tuple(map(sum, zip(*map(lambda a: [1] * a + [0] * (board[0] - a), board))))
def sim(board: tuple, output, auto=True)->tuple:
    board = trim(board)
    length = len(board)
    output.flipped = False
    if auto:
        if length > 0 and board[0] < length:
            output.flipped = True
            board = flip(board)
        elif length > 0 and board[0] == length:
            other = flip(board)
            if other > board:
                output.flipped = True
                board = other
    return board

class Board():
    def __init__(self, data, auto=True):
        self.data = sim(tuple(data), self, auto)
        if auto:
            other = self.clone(False)._flip()
            loth = len(other.data)
            lsel = len(self.data)
            if lsel > loth or lsel == loth and self.data > other.data:
                self.data = other.data
                self.flipped = other.flipped
    def clone(self, auto=True):
        return Board(self.data, auto)
    
    def _flip(self):
        self.flipped = not self.flipped
        self.data = tuple(flip(self.data))
        return self

    def chomp(self, x, y, auto=True):
        data = list(self.data)
        for row in range(len(data)):
            if x < data[row] and y <= row:
                data[row] = x
        res = Board(data, auto)
        return res
    
    def __getitem__(self, index):
        return self.data[index]

    def __len__(self):
        return len(self.data)

    def __str__(self):
        return str(self.data)
    
    def __hash__(self):
        return hash(self.data)

    def __eq__(self, other):
        if not isinstance(other, Board):
            return False
        return self.data == other.data

    def as_ideal(self):
        row = 0
        last = self[0] + 1
        gens = []
        for row, col in enumerate(self.data):
            if col != last:
                last = col
                gens.append(f'({last},{row})')
        gens.append(f'({0},{len(self)})')
        return '<' + ','.join(gens) + '>'

    def can_reach(self, other, traced=False):
        if len(other) == len(self):
            valid = False
            for i in range(len(self)):
                if other[i] > self[i]: 
                    valid = False
                    break
                if other[i] < self[i]:
                    if valid == True:
                        if other[i] != other[i - 1]:
                            valid = False
                            break
                    else:
                        valid = True
            if valid: 
                return True
        if len(other) < len(self):
            for a, b in zip(other.data, self.data):
                if a != b: break
            else:
                return True
        if traced:
            return False
        else:
            return self.can_reach(other.clone(False)._flip(), True)

    @staticmethod
    def from_ideal(ideal, auto=True):
        s = ideal[1:-1].replace(' ', '')
        r = []
        v = 0
        c = 0
        ss = s.split(',')
        for x, y in zip(ss[::2], ss[1::2]):
            x = int(x[1:])
            y = int(y[:-1])
            while c < y:
                c += 1
                r += [v]
            v = x
        return Board(tuple(r), auto=auto)