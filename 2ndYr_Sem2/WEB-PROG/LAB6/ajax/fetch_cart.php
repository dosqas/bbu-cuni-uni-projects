<?php
require '../includes/db.php'; // Include the database connection

// Check if the request is a GET request
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Optional: Filter by category or search term
    $category = isset($_GET['category']) ? $_GET['category'] : null;

    try {
        // Prepare the SQL query
        if ($category) {
            $stmt = $pdo->prepare("SELECT * FROM products WHERE category = :category");
            $stmt->execute(['category' => $category]);
        } else {
            $stmt = $pdo->query("SELECT * FROM products");
        }

        // Fetch all products
        $products = $stmt->fetchAll(PDO::FETCH_ASSOC);

        // Return the products as a JSON response
        header('Content-Type: application/json');
        echo json_encode(['success' => true, 'products' => $products]);
    } catch (PDOException $e) {
        // Handle database errors
        echo json_encode(['success' => false, 'message' => 'Failed to fetch products: ' . $e->getMessage()]);
    }
    exit;
}

// If the request is not GET, return an error
echo json_encode(['success' => false, 'message' => 'Invalid request method.']);
exit;
?>