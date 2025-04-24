<?php
require '../includes/db.php'; // Include the database connection

try {
    // Fetch all categories
    $stmt = $pdo->query("SELECT id, name FROM categories");
    $categories = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Return the categories as a JSON response
    echo json_encode(['success' => true, 'categories' => $categories]);
} catch (PDOException $e) {
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
}