package com.dosqas.services;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class PuzzleService {
    private static final int SIZE = 3;

    private static final int[][] SOLVED_BOARD = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 0}
    };

    public static int[][] initializeBoard() {
        Integer[] numbers = new Integer[SIZE * SIZE];
        for (int i = 0; i < numbers.length; i++) {
            numbers[i] = i; // 0 represents the empty space
        }

        // Shuffle until we get a solvable configuration
        do {
            List<Integer> list = Arrays.asList(numbers);
            Collections.shuffle(list);
            list.toArray(numbers);
        } while (!isSolvable(numbers));

        // Convert to 2D array
        int[][] board = new int[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = numbers[i * SIZE + j];
            }
        }

        return board;
    }

    private static boolean isSolvable(Integer[] puzzle) {
        int inversions = 0;
        for (int i = 0; i < puzzle.length - 1; i++) {
            for (int j = i + 1; j < puzzle.length; j++) {
                if (puzzle[i] > puzzle[j] && puzzle[i] != 0 && puzzle[j] != 0) {
                    inversions++;
                }
            }
        }
        return inversions % 2 == 0;
    }

    // Move a tile to the empty space (0)
    // Returns true if move was successful, false otherwise
    public static boolean moveTile(int[][] board, int tileValue) {
        int[] emptyPos = findPosition(board, 0);
        int[] tilePos = findPosition(board, tileValue);

        // Check if the tile is adjacent to the empty space
        if (isAdjacent(emptyPos, tilePos)) {
            // Swap them
            board[emptyPos[0]][emptyPos[1]] = tileValue;
            board[tilePos[0]][tilePos[1]] = 0;
            return true;
        }
        return false;
    }

    // Helper: Find position of a value in the board
    private static int[] findPosition(int[][] board, int value) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == value) {
                    return new int[]{i, j};
                }
            }
        }
        return new int[]{-1, -1};
    }

    // Helper: Check if two positions are adjacent
    private static boolean isAdjacent(int[] pos1, int[] pos2) {
        return (Math.abs(pos1[0] - pos2[0]) == 1 && pos1[1] == pos2[1]) ||
                (Math.abs(pos1[1] - pos2[1]) == 1 && pos1[0] == pos2[0]);
    }

    public static int[][] loadGameState(int userId, Connection conn) throws SQLException {
        String sql = "SELECT board_state FROM puzzle_games WHERE user_id = ? AND completed = FALSE";
        try (PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setInt(1, userId);
            ResultSet rs = stmt.executeQuery();
            if (rs.next()) {
                return parseBoardState(rs.getString("board_state"));
            }
        }
        return null;
    }

    public static int getCurrentMoves(int userId, Connection conn) throws SQLException {
        String sql = "SELECT moves FROM puzzle_games WHERE user_id = ? AND completed = FALSE";
        try (PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setInt(1, userId);
            ResultSet rs = stmt.executeQuery();
            return rs.next() ? rs.getInt("moves") : 0;
        }
    }

    public static void saveGameState(int userId, int[][] board, int moves, Connection conn) throws SQLException {
        // First check if there's already a game in progress
        String checkSql = "SELECT id FROM puzzle_games WHERE user_id = ? AND completed = FALSE";
        try (PreparedStatement checkStmt = conn.prepareStatement(checkSql)) {
            checkStmt.setInt(1, userId);
            ResultSet rs = checkStmt.executeQuery();

            if (rs.next()) {
                // Update existing game
                updateGameState(userId, board, moves, conn);
                return;
            }
        }

        // Insert new game
        String sql = "INSERT INTO puzzle_games (user_id, board_state, moves, completed) VALUES (?, ?, ?, FALSE)";
        try (PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.setString(2, convertBoardToString(board));
            stmt.setInt(3, moves);
            stmt.executeUpdate();
        }
    }

    public static void updateGameState(int userId, int[][] board, int moves, Connection conn) throws SQLException {
        String sql = "UPDATE puzzle_games SET board_state = ?, moves = ? WHERE user_id = ? AND completed = FALSE";
        try (PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setString(1, convertBoardToString(board));
            stmt.setInt(2, moves);
            stmt.setInt(3, userId);
            stmt.executeUpdate();
        }
    }

    public static void markGameCompleted(int userId, Connection conn) throws SQLException {
        String sql = "UPDATE puzzle_games SET completed = TRUE WHERE user_id = ? AND completed = FALSE";
        try (PreparedStatement stmt = conn.prepareStatement(sql)) {
            stmt.setInt(1, userId);
            stmt.executeUpdate();
        }
    }

    // Utility methods
    private static String convertBoardToString(int[][] board) {
        StringBuilder sb = new StringBuilder();
        for (int[] row : board) {
            for (int val : row) {
                sb.append(val).append(",");
            }
        }
        return sb.substring(0, sb.length() - 1); // Remove trailing comma
    }

    private static int[][] parseBoardState(String boardState) {
        String[] values = boardState.split(",");
        int[][] board = new int[SIZE][SIZE];
        for (int i = 0; i < SIZE * SIZE; i++) {
            board[i/SIZE][i%SIZE] = Integer.parseInt(values[i]);
        }
        return board;
    }

    public static boolean isSolvedOptimized(int[][] board) {
        return Arrays.deepToString(board).equals(Arrays.deepToString(SOLVED_BOARD));
    }
}