CREATE DATABASE ecommerce;

USE ecommerce;

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
    FOREIGN KEY (category_id) REFERENCES categories(id) ON DELETE SET NULL
);

INSERT INTO products (name, category_id, price) 
VALUES
('Smartphone', 1, 499.99),
('Laptop', 1, 899.99),
('T-shirt', 2, 19.99),
('Refrigerator', 3, 399.99),
('Novel Book', 4, 14.99),
('Toy Car', 5, 9.99);

SELECT * FROM categories;
SELECT * FROM products;

CREATE TABLE cart (
    id INT AUTO_INCREMENT PRIMARY KEY,
    product_id INT NOT NULL, -- Foreign key to the products table
    quantity INT NOT NULL DEFAULT 1,
    FOREIGN KEY (product_id) REFERENCES products(id)
);