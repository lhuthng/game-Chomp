def iterator(board):
    for row in range(len(board)):
        for col in range(board[row]):
            if (col, row) != (0, 0):
                yield (col, row)

def reverse_iterator(board):
    for row in range(len(board)-1, -1, -1):
        for col in range(board[row]-1, -1, -1):
            if (col, row) != (0, 0):
                yield (col, row)

def zigzag_iterator(board):
    x, y = 0, 0
    length = len(board)
    limit = max([i + e for i, e in enumerate(board)]) + 1
    while x + y < limit:
        x, y = x - 1, y + 1
        if x == -1:
            x, y = x + y + 1, 0
        if y > length:
            x, y = 0, x + y
        if y < length and x < board[y]:
            yield (x, y)
