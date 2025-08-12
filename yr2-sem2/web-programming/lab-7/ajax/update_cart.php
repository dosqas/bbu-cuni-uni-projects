<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, GET, PATCH, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');
require '../includes/db.php'; // Include the database connection

// Check if the request is a PATCH request
if ($_SERVER['REQUEST_METHOD'] === 'PATCH') {
    // Get the raw PATCH data (assuming JSON is sent)
    $input = file_get_contents('php://input');
    $data = json_decode($input, true);

    // Validate the product ID and quantity
    if (!isset($data['id']) || !is_numeric($data['id']) || !isset($data['quantity']) || !is_numeric($data['quantity']) || $data['quantity'] < 1) {
        echo json_encode(['success' => false, 'message' => 'Invalid product ID or quantity.']);
        exit;
    }

    $productId = (int)$data['id'];
    $quantity = (int)$data['quantity'];

    try {
        // Check if the product exists in the cart
        $stmt = $pdo->prepare("SELECT * FROM cart WHERE product_id = :product_id");
        $stmt->execute(['product_id' => $productId]);
        $cartItem = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($cartItem) {
            // Update the quantity of the product in the cart
            $stmt = $pdo->prepare("UPDATE cart SET quantity = :quantity WHERE product_id = :product_id");
            $stmt->execute(['quantity' => $quantity, 'product_id' => $productId]);
            echo json_encode(['success' => true, 'message' => 'Cart updated successfully.']);
        } else {
            echo json_encode(['success' => false, 'message' => 'Product not found in cart.']);
        }
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

// If the request is not PATCH, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>