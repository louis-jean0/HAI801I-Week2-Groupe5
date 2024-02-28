import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;

public class minmax_Brian {
    public static int evaluate(char[][] board) {
        for (int row = 0; row < 3; row++) {
            if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
                if (board[row][0] == 'X') {
                    return 10;
                } else if (board[row][0] == 'O') {
                    return -10;
                }
            }
        }

        for (int col = 0; col < 3; col++) {
            if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
                if (board[0][col] == 'X') {
                    return 10;
                } else if (board[0][col] == 'O') {
                    return -10;
                }
            }
        }

        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            if (board[0][0] == 'X') {
                return 10;
            } else if (board[0][0] == 'O') {
                return -10;
            }
        }

        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            if (board[0][2] == 'X') {
                return 10;
            } else if (board[0][2] == 'O') {
                return -10;
            }
        }

        return 0;
    }

    public static boolean isMovesLeft(char[][] board) {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == ' ') {
                    return true;
                }
            }
        }
        return false;
    }

    public static int minimax(char[][] board, int depth, boolean isMax) {
        int score = evaluate(board);

        if (score == 10) {
            return score;
        }

        if (score == -10) {
            return score;
        }

        if (!isMovesLeft(board)) {
            return 0;
        }

        if (isMax) {
            int best = -1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'X';
                        best = Math.max(best, minimax(board, depth + 1, !isMax));
                        board[i][j] = ' ';
                    }
                }
            }
            return best;
        } else {
            int best = 1000;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') {
                        board[i][j] = 'O';
                        best = Math.min(best, minimax(board, depth + 1, !isMax));
                        board[i][j] = ' ';
                    }
                }
            }
            return best;
        }
    }

    public static int[] bestMove(char[][] board, char player) {
        int bestVal = (player == 'X') ? -1000 : 1000;
        int[] bestMove = {-1, -1};

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;
                    int moveVal = minimax(board, 9, false);
                    board[i][j] = ' ';
                    if ((player == 'X' && moveVal > bestVal) || (player == 'O' && moveVal < bestVal)) {
                        bestMove[0] = i;
                        bestMove[1] = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
        return bestMove;
    }

    public static int simulateGame(char[][] board, char player) {
        while (isMovesLeft(board) && evaluate(board) == 0) {
            int[] move = bestMove(board, player);
            if (move[0] != -1 && move[1] != -1) {
                board[move[0]][move[1]] = player;
                player = (player == 'X') ? 'O' : 'X';
            } else {
                break;
            }
        }
        return evaluate(board);
    }

    public static void main(String[] args) {
        int winsForX = 0;
        int draws = 0;
        int winsForO = 0;
        int totalGames = 0;
        long startTime = System.currentTimeMillis();

        try (BufferedReader reader = new BufferedReader(new FileReader("dataset.txt"))) {
            String line;
            while ((line = reader.readLine()) != null) {
                totalGames++;
                char player = line.charAt(0);
                char[][] board = new char[3][3];
                for (int i = 0; i < 3; i++) {
                    board[i] = line.substring(1 + i * 3, 4 + i * 3).toCharArray();
                }
                int result = simulateGame(board, player);
                if (result == 10) {
                    winsForX++;
                } else if (result == -10) {
                    winsForO++;
                } else {
                    draws++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        long endTime = System.currentTimeMillis();
        double elapsedTime = (endTime - startTime) / 1000.0;

        System.out.println("Nombre total de parties gagnées par 'X' : " + winsForX + ", soit " + (int) ((winsForX / (double) totalGames) * 100) + "%");
        System.out.println("Nombre total de parties gagnées par 'O' : " + winsForO + ", soit " + (int) ((winsForO / (double) totalGames) * 100) + "%");
        System.out.println("Nombre total de parties nulles : " + draws + ", soit " + (int) ((draws / (double) totalGames) * 100) + "%");
        System.out.println("Temps écoulé : " + elapsedTime + "s");
    }
}
