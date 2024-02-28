#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

int evaluate(std::vector<std::vector<char>>& board) {
    for (int row = 0; row < 3; ++row) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == 'X')
                return 10;
            else if (board[row][0] == 'O')
                return -10;
        }
    }

    for (int col = 0; col < 3; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == 'X')
                return 10;
            else if (board[0][col] == 'O')
                return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X')
            return 10;
        else if (board[0][0] == 'O')
            return -10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X')
            return 10;
        else if (board[0][2] == 'O')
            return -10;
    }

    return 0;
}

bool is_moves_left(const std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == ' ')
                return true;
        }
    }
    return false;
}

int minimax(std::vector<std::vector<char>>& board, int depth, bool is_max);

std::pair<int, int> best_move(std::vector<std::vector<char>>& board, char player);

int simulate_game(std::vector<std::vector<char>>& board, char player);

int main() {
    int wins_for_x = 0;
    int draw = 0;
    int wins_for_o = 0;
    int cpt = 0;
    clock_t start = clock();
    std::ifstream file("dataset.txt");

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        cpt++;
        char player = line[0];
        std::vector<std::vector<char>> board(3, std::vector<char>(3));
        for (int i = 1; i < 10; i += 3) {
            for (int j = 0; j < 3; ++j) {
                board[i / 3][j] = line[i + j];
            }
        }
        int result = simulate_game(board, player);
        if (result == 10)
            wins_for_x++;
        else if (result == -10)
            wins_for_o++;
        else if (result == 0)
            draw++;
    }

    clock_t end = clock();
    double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    int pourcentage_x = static_cast<int>((wins_for_x / static_cast<double>(cpt)) * 100);
    int pourcentage_o = static_cast<int>((wins_for_o / static_cast<double>(cpt)) * 100);
    int pourcentage_draw = static_cast<int>((draw / static_cast<double>(cpt)) * 100);
    std::cout << "Nombre total de parties gagnées par 'X' : " << wins_for_x << " soit " << pourcentage_x << "%" << std::endl;
    std::cout << "Nombre total de parties gagnées par 'O' : " << wins_for_o << " soit " << pourcentage_o << "%" << std::endl;
    std::cout << "Nombre total de parties nulles : " << draw << " soit " << pourcentage_draw << "%" << std::endl;
    std::cout << "Temps écoulé : " << elapsed << "s" << std::endl;

    return 0;
}

int minimax(std::vector<std::vector<char>>& board, int depth, bool is_max) {
    int score = evaluate(board);

    if (score == 10)
        return score;

    if (score == -10)
        return score;

    if (!is_moves_left(board))
        return 0;

    if (is_max) {
        int best = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = std::max(best, minimax(board, depth + 1, !is_max));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = std::min(best, minimax(board, depth + 1, !is_max));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

std::pair<int, int> best_move(std::vector<std::vector<char>>& board, char player) {
    int best_val = (player == 'X') ? -1000 : 1000;
    std::pair<int, int> best_move = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int move_val = minimax(board, 9, false);
                board[i][j] = ' ';
                if ((player == 'X' && move_val > best_val) || (player == 'O' && move_val < best_val)) {
                    best_move = {i, j};
                    best_val = move_val;
                }
            }
        }
    }

    return best_move;
}

int simulate_game(std::vector<std::vector<char>>& board, char player) {
    while (is_moves_left(board) && evaluate(board) == 0) {
        auto move = best_move(board, player);
        if (move != std::make_pair(-1, -1)) {
            board[move.first][move.second] = player;
            player = (player == 'X') ? 'O' : 'X';
        } else {
            break;
        }
    }
    return evaluate(board);
}
