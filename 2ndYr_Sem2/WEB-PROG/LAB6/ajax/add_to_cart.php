<?php
require '../includes/db.php'; // Include the database connection

// Check if the request is a POST request
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
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
        // Check if the product already exists in the cart
        $stmt = $pdo->prepare("SELECT * FROM cart WHERE product_id = :product_id");
        $stmt->execute(['product_id' => $productId]);
        $cartItem = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($cartItem) {
            // If the product is already in the cart, increase the quantity
            $stmt = $pdo->prepare("UPDATE cart SET quantity = quantity + 1 WHERE product_id = :product_id");
            $stmt->execute(['product_id' => $productId]);
        } else {
            // Otherwise, add the product to the cart with a quantity of 1
            $stmt = $pdo->prepare("INSERT INTO cart (product_id, quantity) VALUES (:product_id, 1)");
            $stmt->execute(['product_id' => $productId]);
        }

        echo json_encode(['success' => true, 'message' => 'Product added to cart.']);
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

// If the request is not POST, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>