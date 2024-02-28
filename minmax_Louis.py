import os

def minimax(board, is_player_x):
    
    pass

def process_game_state(game_state):
    
    player = game_state[0]
    board = game_state[1:]
    
    
    structured_board = [list(board[i:i+3]) for i in range(0, 9, 3)]
    
    is_player_x = player == 'X'
    
    best_move = minimax(structured_board, is_player_x)
    
    return best_move

def main():
    game_states = []
    
    file_path = 'dataset.txt'
    
    with open(file_path, 'r') as file:
        game_states = file.readlines()
    
    for game_state in game_states:
        game_state = game_state.strip()
        best_move = process_game_state(game_state)
        print(f"Best move for game state {game_state}: {best_move}")

main()