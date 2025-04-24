<?php
require '../includes/db.php'; // Include the database connection

if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    $input = file_get_contents('php://input');
    $data = json_decode($input, true);

    // Validate input
    if (!isset($data['id']) || !is_numeric($data['id'])) {
        echo json_encode(['success' => false, 'message' => 'Invalid product ID.']);
        exit;
    }

    $productId = (int)$data['id'];

    try {
        // Delete the product from the database
        $stmt = $pdo->prepare("DELETE FROM products WHERE id = :id");
        $stmt->execute(['id' => $productId]);

        if ($stmt->rowCount() > 0) {
            echo json_encode(['success' => true, 'message' => 'Product deleted successfully.']);
        } else {
            echo json_encode(['success' => false, 'message' => 'Product not found.']);
        }
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;