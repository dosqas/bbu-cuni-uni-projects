<%@ page contentType="text/html;charset=UTF-8" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
<head>
    <title>Image Puzzle</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            margin: 20px;
            background-color: #f5f5f5;
        }

        header {
            width: 100%;
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 10px 20px;
            background-color: white;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            margin-bottom: 20px;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        button {
            all: unset;
            cursor: pointer;
            display: block;
        }

        .user-info {
            display: flex;
            align-items: center;
        }

        .username {
            font-weight: bold;
            margin-right: 15px;
        }

        .puzzle-board {
            display: grid;
            grid-template-columns: repeat(3, 100px);
            gap: 2px;
            border: 3px solid #333;
            box-shadow: 0 0 10px rgba(0,0,0,0.2);
            margin-bottom: 20px;
        }

        .tile {
            width: 100px;
            height: 100px;
            position: relative;
            padding: 0;
            border: none;
            background: none;
        }

        .tile-content {
            width: 100%;
            height: 100%;
            background-image: url('images/puzzle.jpg');
            background-size: 300px 300px;
            background-repeat: no-repeat;
        }

        .empty {
            background: #ddd;
        }

        .game-info {
            margin-bottom: 20px;
            font-size: 1.2em;
            background-color: white;
            padding: 15px;
            border-radius: 8px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            text-align: center;
        }

        .btn {
            background-color: #4CAF50;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            margin: 10px 5px;
            transition: background-color 0.3s;
            text-align: center;
        }

        .btn:hover {
            background-color: #45a049;
        }

        .btn.secondary {
            background-color: #607d8b;
        }

        .btn.secondary:hover {
            background-color: #546e7a;
        }

        .btn.danger {
            background-color: #f44336;
        }

        .btn.danger:hover {
            background-color: #d32f2f;
        }

        .controls {
            display: flex;
            justify-content: center;
            margin-bottom: 20px;
        }

        .win-message {
            color: #4CAF50;
            font-size: 1.5em;
            font-weight: bold;
            text-align: center;
            padding: 20px;
            background-color: white;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0,0,0,0.1);
            display: ${sessionScope.win ? 'block' : 'none'};
            margin-bottom: 20px;
            animation: fadeIn 1s;
        }

        @keyframes fadeIn {
            from { opacity: 0; }
            to { opacity: 1; }
        }
    </style>
</head>
<body>
<header>
    <h1>Image Puzzle Game</h1>
    <div class="user-info">
        <span class="username">Welcome, ${sessionScope.user}!</span>
        <form action="logout" method="post" style="margin:0">
            <button type="submit" class="btn secondary">Logout</button>
        </form>
    </div>
</header>

<c:if test="${!sessionScope.win}">
    <div class="game-info">
        <p>Moves: <strong>${sessionScope.moves}</strong></p>
    </div>
</c:if>

<c:if test="${sessionScope.win}">
    <div class="win-message">
        🎉 Congratulations! You solved the puzzle in ${sessionScope.winMoves} moves! 🎉
    </div>
</c:if>

<div class="puzzle-board" style="${sessionScope.win ? 'opacity: 0.7;' : ''}">
    <c:forEach var="row" items="${sessionScope.board}" varStatus="rowStatus">
        <c:forEach var="tile" items="${row}" varStatus="colStatus">
            <c:choose>
                <c:when test="${tile == 0}">
                    <div class="tile empty"></div>
                </c:when>
                <c:otherwise>
                    <form action="puzzle" method="post" style="margin:0;padding:0;">
                        <input type="hidden" name="tile" value="${tile}">
                        <button type="submit" class="tile" ${sessionScope.win ? 'disabled' : ''}>
                            <div class="tile-content"
                                 style="background-position: ${-((tile-1)%3)*100}px ${-Math.floor((tile-1)/3)*100}px;"></div>
                        </button>
                    </form>
                </c:otherwise>
            </c:choose>
        </c:forEach>
    </c:forEach>
</div>

<div class="controls">
    <form action="puzzle" method="get">
        <input type="hidden" name="newGame" value="true">
        <button type="submit" class="btn">New Game</button>
    </form>

    <c:if test="${sessionScope.highScores != null}">
        <button type="button" class="btn secondary" id="showScores">High Scores</button>
    </c:if>
</div>

</body>
</html>