<%@ page contentType="text/html;charset=UTF-8" %>
<!DOCTYPE html>
<html>
<head>
    <title>Image Puzzle - Login/Register</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f5f5f5;
        }

        .container {
            background-color: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            width: 400px;
        }

        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 20px;
        }

        .tabs {
            display: flex;
            border-bottom: 1px solid #ddd;
            margin-bottom: 20px;
        }

        .tab {
            padding: 10px 20px;
            cursor: pointer;
            flex: 1;
            text-align: center;
            transition: background-color 0.3s;
        }

        .tab.active {
            border-bottom: 2px solid #4CAF50;
            color: #4CAF50;
            font-weight: bold;
        }

        .tab:hover:not(.active) {
            background-color: #f9f9f9;
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
        }

        .form-group {
            margin-bottom: 20px;
        }

        label {
            display: block;
            margin-bottom: 8px;
            font-weight: bold;
        }

        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            box-sizing: border-box;
        }

        button {
            width: 100%;
            padding: 12px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
        }

        button:hover {
            background-color: #45a049;
        }

        .message {
            margin-bottom: 15px;
            text-align: center;
            padding: 10px;
            border-radius: 4px;
        }

        .error-message {
            color: white;
            background-color: #f44336;
        }

        .success-message {
            color: white;
            background-color: #4CAF50;
        }
    </style>
</head>
<body>
<div class="container">
    <h1>Image Puzzle</h1>

    <div class="tabs">
        <div id="login-tab" class="tab active" onclick="switchTab('login')">Login</div>
        <div id="register-tab" class="tab" onclick="switchTab('register')">Register</div>
    </div>

    <!-- Login Form -->
    <div id="login-content" class="tab-content active">
        <% if (request.getAttribute("error") != null) { %>
        <div class="message error-message">
            <%= request.getAttribute("error") %>
        </div>
        <% } %>

        <% if (request.getParameter("regSuccess") != null) { %>
        <div class="message success-message">
            Registration successful! Please login.
        </div>
        <% } %>

        <form action="login" method="post">
            <div class="form-group">
                <label for="username">Username:</label>
                <input type="text" id="username" name="username" required>
            </div>

            <div class="form-group">
                <label for="password">Password:</label>
                <input type="password" id="password" name="password" required>
            </div>

            <button type="submit">Login</button>
        </form>
    </div>

    <!-- Registration Form -->
    <div id="register-content" class="tab-content">
        <%
            String regError = request.getParameter("regError");
            if (regError != null) {
                String errorMessage = "";
                if (regError.equals("mismatch")) {
                    errorMessage = "Passwords do not match!";
                } else if (regError.equals("exists")) {
                    errorMessage = "Username already exists!";
                } else if (regError.equals("db")) {
                    errorMessage = "Database error. Please try again.";
                }
        %>
        <div class="message error-message">
            <%= errorMessage %>
        </div>
        <% } %>

        <form action="register" method="post">
            <div class="form-group">
                <label for="reg-username">Username:</label>
                <input type="text" id="reg-username" name="username" required>
            </div>

            <div class="form-group">
                <label for="reg-password">Password:</label>
                <input type="password" id="reg-password" name="password" required>
            </div>

            <div class="form-group">
                <label for="confirm-password">Confirm Password:</label>
                <input type="password" id="confirm-password" name="confirmPassword" required>
            </div>

            <button type="submit">Register</button>
        </form>
    </div>
</div>

<script>
    // Check if tab parameter is in URL and switch to that tab
    window.onload = function() {
        const urlParams = new URLSearchParams(window.location.search);
        const tab = urlParams.get('tab');
        if (tab === 'register') {
            switchTab('register');
        }
    };

    function switchTab(tabName) {
        // Hide all tabs and content
        document.querySelectorAll('.tab').forEach(tab => {
            tab.classList.remove('active');
        });
        document.querySelectorAll('.tab-content').forEach(content => {
            content.classList.remove('active');
        });

        // Show selected tab and content
        document.getElementById(tabName + '-tab').classList.add('active');
        document.getElementById(tabName + '-content').classList.add('active');
    }
</script>
</body>
</html>