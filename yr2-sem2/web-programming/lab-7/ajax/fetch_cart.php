<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, GET, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');
require '../includes/db.php'; // Include the database connection

try {
    // Fetch all items in the cart
    $stmt = $pdo->query("SELECT c.product_id, c.quantity, p.name, p.price 
                         FROM cart c 
                         JOIN products p ON c.product_id = p.id");
    $cartItems = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Debugging: Output the raw data
    header('Content-Type: application/json');
    echo json_encode(['success' => true, 'cart' => $cartItems]);
} catch (PDOException $e) {
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
}