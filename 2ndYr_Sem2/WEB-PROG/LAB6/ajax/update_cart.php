<?php
session_start(); // Start the session to store cart data

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

    $productId = $data['id'];
    $quantity = (int)$data['quantity'];

    // Check if the cart exists in the session
    if (!isset($_SESSION['cart']) || !isset($_SESSION['cart'][$productId])) {
        echo json_encode(['success' => false, 'message' => 'Product not found in cart.']);
        exit;
    }

    // Update the quantity of the product in the cart
    $_SESSION['cart'][$productId]['quantity'] = $quantity;

    // Return a success response
    echo json_encode(['success' => true, 'message' => 'Cart updated successfully.']);
    exit;
}

// If the request is not PATCH, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>