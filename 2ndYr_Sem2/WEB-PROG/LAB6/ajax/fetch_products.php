<?php
require '../includes/db.php'; // Include the database connection

// Get the category and page parameters
$category = isset($_GET['category']) ? (int)$_GET['category'] : null;
$page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
$limit = 4; // Number of products per page
$offset = ($page - 1) * $limit;

try {
    // Build the SQL query with optional category filtering
    $sql = "SELECT 
                p.id AS product_id, 
                p.name AS product_name, 
                p.price, 
                p.description, 
                p.image_url, 
                c.name AS category_name
            FROM products p
            JOIN categories c ON p.category_id = c.id";
    if ($category) {
        $sql .= " WHERE p.category_id = :category";
    }
    $sql .= " LIMIT :limit OFFSET :offset";

    $stmt = $pdo->prepare($sql);

    // Bind parameters
    if ($category) {
        $stmt->bindParam(':category', $category, PDO::PARAM_INT);
    }
    $stmt->bindParam(':limit', $limit, PDO::PARAM_INT);
    $stmt->bindParam(':offset', $offset, PDO::PARAM_INT);

    $stmt->execute();
    $products = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Check if there are more products for the next page
    $countSql = "SELECT COUNT(*) FROM products";
    if ($category) {
        $countSql .= " WHERE category_id = :category";
    }
    $countStmt = $pdo->prepare($countSql);
    if ($category) {
        $countStmt->bindParam(':category', $category, PDO::PARAM_INT);
    }
    $countStmt->execute();
    $totalProducts = $countStmt->fetchColumn();

    $hasMore = $totalProducts > $page * $limit;

    // Return the products and pagination info as a JSON response
    echo json_encode(['success' => true, 'products' => $products, 'hasMore' => $hasMore]);
} catch (PDOException $e) {
    echo json_encode(['success' => false, 'message' => 'Database error: ' . $e->getMessage()]);
}