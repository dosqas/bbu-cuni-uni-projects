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

    // Check if the cart exists in the session, if not, initialize it
    if (!isset($_SESSION['cart'])) {
        $_SESSION['cart'] = [];
    }

    // Check if the product is already in the cart
    if (isset($_SESSION['cart'][$productId])) {
        // If the product is already in the cart, increase the quantity
        $_SESSION['cart'][$productId]['quantity'] += 1;
    } else {
        // Otherwise, add the product to the cart with a quantity of 1
        $_SESSION['cart'][$productId] = [
            'id' => $productId,
            'quantity' => 1
        ];
    }

    // Return a success response
    echo json_encode(['success' => true, 'message' => 'Product added to cart.']);
    exit;
}

// If the request is not POST, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>