CREATE DATABASE ecommerce_auth;

USE ecommerce_auth;

# Ecommerce
CREATE TABLE categories (
	id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

INSERT INTO categories (name) 
VALUES 
('Electronics'),
('Clothing'),
('Home Appliances'),
('Books'),
('Toys');

CREATE TABLE products (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    description TEXT,
    image_url VARCHAR(255),
    category_id INT,
    FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE CASCADE
);

INSERT INTO products (name, category_id, price, description) 
VALUES
('Smartphone', 1, 499.99, 'Sample description'),
('Laptop', 1, 899.99, 'Sample description'),
('T-shirt', 2, 19.99, 'Sample description'),
('Refrigerator', 3, 399.99, 'Sample description'),
('Novel Book', 4, 14.99, 'Sample description'),
('Toy Car', 5, 9.99, 'Sample description');

CREATE TABLE cart (
    id INT AUTO_INCREMENT PRIMARY KEY,
    product_id INT NOT NULL, -- Foreign key to the products table
    quantity INT NOT NULL DEFAULT 1,
    FOREIGN KEY (product_id) REFERENCES products(id) ON DELETE CASCADE
);

SELECT * FROM categories;
SELECT * FROM products;
SELECT * FROM cart;

DROP TABLE categories;
DROP TABLE products;
DROP TABLE cart;