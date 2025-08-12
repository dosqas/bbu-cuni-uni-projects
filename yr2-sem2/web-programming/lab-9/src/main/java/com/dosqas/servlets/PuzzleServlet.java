package com.dosqas.servlets;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;

import static com.dosqas.services.PuzzleService.*;
import static com.dosqas.utils.DatabaseUtil.getDatabaseConnection;

@WebServlet("/puzzle")
public class PuzzleServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        HttpSession session = request.getSession();
        Integer userId = (Integer) session.getAttribute("userId");

        if (userId == null) {
            response.sendRedirect("index.jsp");
            return;
        }

        String newGame = request.getParameter("newGame");
        boolean startNewGame = newGame != null && newGame.equals("true");

        try (Connection conn = getDatabaseConnection()) {
            int[][] board;
            int moves;

            // Check for existing win state to preserve
            Boolean winState = (Boolean) session.getAttribute("win");

            if (startNewGame) {
                // Start a new game explicitly
                session.removeAttribute("win");
                board = initializeBoard();
                moves = 0;
                saveGameState(userId, board, moves, conn);
            } else if (winState != null && winState) {
                // If the user has already won, preserve the current board and moves
                board = (int[][]) session.getAttribute("board");
                moves = (int) session.getAttribute("moves");
            } else if ((board = loadGameState(userId, conn)) == null) {
                // No existing game found, initialize a new one
                board = initializeBoard();
                moves = 0;
                saveGameState(userId, board, moves, conn);
            } else {
                // Use existing game state
                moves = getCurrentMoves(userId, conn);

                // Check for win condition if not already marked
                if (winState == null && isSolvedOptimized(board)) {
                    session.setAttribute("win", true);
                    session.setAttribute("winMoves", moves);
                }
            }

            session.setAttribute("board", board);
            session.setAttribute("moves", moves);

            // Store the original moves when we win for display purposes
            if (winState == null && isSolvedOptimized(board)) {
                session.setAttribute("winMoves", moves);
            }

            request.getRequestDispatcher("puzzle.jsp").forward(request, response);

        } catch (SQLException e) {
            response.sendError(500, "Database error: " + e.getMessage());
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws IOException {

        HttpSession session = request.getSession();
        Integer userId = (Integer) session.getAttribute("userId");

        if (userId == null) {
            response.sendRedirect("index.jsp");
            return;
        }

        // Don't process moves if the puzzle is already solved
        Boolean win = (Boolean) session.getAttribute("win");
        if (win != null && win) {
            response.sendRedirect("puzzle");
            return;
        }

        try (Connection conn = getDatabaseConnection()) {
            int[][] board = (int[][]) session.getAttribute("board");
            int moves = (int) session.getAttribute("moves");

            int tileValue = Integer.parseInt(request.getParameter("tile"));

            // Try to move the tile
            boolean moved = moveTile(board, tileValue);

            // Only increment moves if an actual move was made
            if (moved) {
                moves++;

                // Check for win condition
                if (isSolvedOptimized(board)) {
                    markGameCompleted(userId, conn);
                    session.setAttribute("win", true);

                    // Store the winning moves for display
                    session.setAttribute("winMoves", moves);
                }

                // Update game state
                updateGameState(userId, board, moves, conn);
                session.setAttribute("moves", moves);
            }

            session.setAttribute("board", board);
            response.sendRedirect("puzzle");

        } catch (SQLException | NumberFormatException e) {
            response.sendError(500, "Game update failed: " + e.getMessage());
        }
    }
}