package com.dosqas.servlets;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;

import static com.dosqas.services.AuthService.createUser;
import static com.dosqas.services.AuthService.userExists;
import static com.dosqas.utils.DatabaseUtil.getDatabaseConnection;

@WebServlet("/register")
public class RegistrationServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String username = request.getParameter("username");
        String password = request.getParameter("password");
        String confirm = request.getParameter("confirmPassword");

        // Check for empty fields
        if (username == null || username.trim().isEmpty() ||
                password == null || password.trim().isEmpty() ||
                confirm == null || confirm.trim().isEmpty()) {
            response.sendRedirect("index.jsp?tab=register&regError=empty");
            return;
        }

        // Validate password match
        if (!password.equals(confirm)) {
            response.sendRedirect("index.jsp?tab=register&regError=mismatch");
            return;
        }

        try (Connection conn = getDatabaseConnection()) {
            // Check if username exists
            if (userExists(conn, username)) {
                response.sendRedirect("index.jsp?tab=register&regError=exists");
                return;
            }

            // Create new user
            createUser(conn, username, password);
            response.sendRedirect("index.jsp?regSuccess=true");

        } catch (SQLException e) {
            e.printStackTrace();
            response.sendRedirect("index.jsp?tab=register&regError=db");
        }
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        // Redirect to registration page
        response.sendRedirect("index.jsp?tab=register");
    }
}