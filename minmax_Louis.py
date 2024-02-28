import os
import time

def evaluate(board):
    for row in range(3):
        if board[row][0] == board[row][1] == board[row][2]:
            if board[row][0] == 'X':
                return 10
            elif board[row][0] == 'O':
                return -10

    for col in range(3):
        if board[0][col] == board[1][col] == board[2][col]:
            if board[0][col] == 'X':
                return 10
            elif board[0][col] == 'O':
                return -10

    if board[0][0] == board[1][1] == board[2][2]:
        if board[0][0] == 'X':
            return 10
        elif board[0][0] == 'O':
            return -10

    if board[0][2] == board[1][1] == board[2][0]:
        if board[0][2] == 'X':
            return 10
        elif board[0][2] == 'O':
            return -10

    return 0

def is_moves_left(board):
    for row in board:
        if ' ' in row:
            return True
    return False

def minimax(board, depth, is_max):
    score = evaluate(board)
    
    if score == 10:
        return score

    if score == -10:
        return score

    if not is_moves_left(board):
        return 0

    if is_max:
        best = -1000
        for i in range(3):
            for j in range(3):
                if board[i][j] == ' ':
                    board[i][j] = 'X'
                    best = max(best, minimax(board, depth + 1, not is_max))
                    board[i][j] = ' '
        return best
    else:
        best = 1000
        for i in range(3):
            for j in range(3):
                
                if board[i][j] == ' ':
                   
                    board[i][j] = 'O'
                    
                    best = min(best, minimax(board, depth + 1, not is_max))
                    
                    board[i][j] = ' '
        return best

def best_move(board, player):
    best_val = -1000 if player == 'X' else 1000
    best_move = (-1, -1)

    for i in range(3):
        for j in range(3):
            if board[i][j] == ' ':
                board[i][j] = player
                move_val = minimax(board, 9, False if player == 'X' else True)
                board[i][j] = ' '
                if (player == 'X' and move_val > best_val) or (player == 'O' and move_val < best_val):
                    best_move = (i, j)
                    best_val = move_val

    return best_move
    
def simulate_game(board, player):
    while is_moves_left(board) and evaluate(board) == 0:
        move = best_move(board, player)
        if move != (-1, -1):  
            board[move[0]][move[1]] = player
            player = 'O' if player == 'X' else 'X' 
        else:
            break  
    return evaluate(board)

def main():
    wins_for_x = 0
    draw = 0
    wins_for_o = 0
    cpt = 0
    start = time.time()
    with open('dataset.txt', 'r') as file:
        for line in file:
            cpt += 1
            player = line[0]
            board = [list(line[i:i+3]) for i in range(1, 10, 3)]
            result = simulate_game(board, player)
            if result == 10: 
                wins_for_x += 1
            if result == -10:
                wins_for_o += 1
            if result == 0:
                draw += 1
    end = time.time()
    elapsed = end - start
    pourcentage_x = round((wins_for_x/cpt)*100)
    pourcentage_o = round((wins_for_o/cpt)*100)
    pourcentage_draw = round((draw/cpt)*100)
    print(f"Nombre total de parties gagnées par 'X' : {wins_for_x}" f" soit {pourcentage_x}%")
    print(f"Nombre total de parties gagnées par 'O' : {wins_for_o}" f" soit {pourcentage_o}%")
    print(f"Nombre total de parties nulles : {draw}" f" soit {pourcentage_draw}%")
    print(f"Temps écoulé : {elapsed}s")

if __name__ == "__main__":
    main()