<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, GET, PUT, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');
require '../includes/db.php'; // Include the database connection

if ($_SERVER['REQUEST_METHOD'] === 'PUT') {
    $input = file_get_contents('php://input');
    
    $data = json_decode($input, true);

    // Validate input
    if (!isset($data['id'], $data['name'], $data['price'], $data['category'], $data['description'])) {
        echo json_encode(['success' => false, 'message' => 'Missing required fields.']);
        exit;
    }

    $id = (int)$data['id'];
    $name = htmlspecialchars(trim($data['name']));
    $price = trim($data['price']);
    $category = (int)trim($data['category']);
    $description = htmlspecialchars(trim($data['description']));

    // Validate name (max 40 characters)
    if (strlen($name) === 0 || strlen($name) > 40) {
        echo json_encode(['success' => false, 'message' => 'Product name must be between 1 and 40 characters.']);
        exit;
    }

    // Validate price (must be a valid number)
    if (!preg_match('/^\d+(\.\d{1,2})?$/', $price)) {
        echo json_encode(['success' => false, 'message' => 'Price must be a valid number (e.g., 10, 10.99).']);
        exit;
    }

    // Validate category (must be a valid integer)
    if ($category <= 0) {
        echo json_encode(['success' => false, 'message' => 'Invalid category selected.']);
        exit;
    }

    // Validate description (max 200 characters)
    if (strlen($description) > 200) {
        echo json_encode(['success' => false, 'message' => 'Description must not exceed 200 characters.']);
        exit;
    }

    try {
        // Update the product in the database
        $stmt = $pdo->prepare("UPDATE products SET name = :name, price = :price, category_id = :category, description = :description WHERE id = :id");
        $stmt->execute([
            'id' => $id,
            'name' => $name,
            'price' => $price,
            'category' => $category,
            'description' => $description
        ]);

        echo json_encode(['success' => true, 'message' => 'Product updated successfully.']);
    } catch (PDOException $e) {
        echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
    }
    exit;
}

echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;