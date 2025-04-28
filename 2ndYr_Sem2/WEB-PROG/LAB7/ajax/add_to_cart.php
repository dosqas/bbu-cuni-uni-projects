<?php
require '../includes/db.php'; // Include the database connection

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $input = file_get_contents('php://input');
    $data = json_decode($input, true);

    // Validate the product ID and quantity
    if (!isset($data['id']) || !is_numeric($data['id']) || !isset($data['quantity']) || !is_numeric($data['quantity'])) {
        echo json_encode(['success' => false, 'message' => 'Invalid product ID or quantity.']);
        exit;
    }

    $productId = (int)$data['id'];
    $quantity = (int)$data['quantity'];

    if ($quantity < 1 || $quantity > 99) {
        echo json_encode(['success' => false, 'message' => 'Quantity must be between 1 and 99.']);
        exit;
    }

    try {
        // Check if the product already exists in the cart
        $stmt = $pdo->prepare("SELECT * FROM cart WHERE product_id = :product_id");
        $stmt->execute(['product_id' => $productId]);
        $cartItem = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($cartItem) {
            // If the product is already in the cart, increase the quantity
            $stmt = $pdo->prepare("UPDATE cart SET quantity = quantity + :quantity WHERE product_id = :product_id");
            $stmt->execute(['quantity' => $quantity, 'product_id' => $productId]);
        } else {
            // Otherwise, add the product to the cart with the specified quantity
            $stmt = $pdo->prepare("INSERT INTO cart (product_id, quantity) VALUES (:product_id, :quantity)");
            $stmt->execute(['product_id' => $productId, 'quantity' => $quantity]);
        }

        echo json_encode(['success' => true, 'message' => 'Product added to cart.']);
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>