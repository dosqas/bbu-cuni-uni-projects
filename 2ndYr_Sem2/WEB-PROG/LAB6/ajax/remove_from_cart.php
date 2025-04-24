<?php
session_start(); // Start the session to store cart data

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

    $productId = $data['id'];

    // Check if the cart exists in the session
    if (!isset($_SESSION['cart']) || !isset($_SESSION['cart'][$productId])) {
        echo json_encode(['success' => false, 'message' => 'Product not found in cart.']);
        exit;
    }

    // Decrease the quantity or remove the product
    if ($_SESSION['cart'][$productId]['quantity'] > 1) {
        $_SESSION['cart'][$productId]['quantity'] -= 1;
        echo json_encode(['success' => true, 'message' => 'Product quantity decreased.']);
    } else {
        unset($_SESSION['cart'][$productId]);
        echo json_encode(['success' => true, 'message' => 'Product removed from cart.']);
    }
    exit;
}

// If the request is not POST, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>