<?php
require '../includes/db.php'; // Include the database connection

// Check if the request is a POST request
if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    // Get the raw POST data (assuming JSON is sent)
    $input = file_get_contents('php://input');
    $data = json_decode($input, true);

    // Validate the product ID
    if (!isset($data['id']) || !is_numeric($data['id'])) {
        echo json_encode(['success' => false, 'message' => 'Invalid product ID.']);
        exit;
    }

    $productId = (int)$data['id'];

    try {
        // Check if the product exists in the cart
        $stmt = $pdo->prepare("SELECT * FROM cart WHERE product_id = :product_id");
        $stmt->execute(['product_id' => $productId]);
        $cartItem = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($cartItem) {
            // Remove the product from the cart
            $stmt = $pdo->prepare("DELETE FROM cart WHERE product_id = :product_id");
            $stmt->execute(['product_id' => $productId]);
            echo json_encode(['success' => true, 'message' => 'Product removed from cart.']);
        } else {
            echo json_encode(['success' => false, 'message' => 'Product not found in cart.']);
        }
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

// If the request is not POST, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>