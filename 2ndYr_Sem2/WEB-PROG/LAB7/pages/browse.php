<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Browse Products</title>
    <style>
        .product {
            border: 1px solid #ccc;
            padding: 10px;
            margin-bottom: 10px;
            max-width: 600px;
            display: flex;
            flex-direction: row;
            align-items: center;
            justify-content: space-between;
        }
        .product img {
            max-width: 150px;
            height: auto;
            margin-left: 20px;
        }
        .product-details {
            display: flex;
            flex-direction: column;
            flex: 1;
        }
        .product-details h2 {
            margin: 0;
            font-size: 1.5em;
        }
        .product-details p {
            margin: 5px 0;
        }
        .product-details button {
            padding: 10px 15px;
            background-color: #007BFF;
            color: white;
            border: none;
            cursor: pointer;
            font-size: 1em;
            align-self: flex-start;
        }
        .product-details button:hover {
            background-color: #0056b3;
        }
        .add-to-cart {
            display: flex;
            align-items: center;
            gap: 10px;
            margin-top: 10px;
        }
        .add-to-cart input {
            width: 50px;
            text-align: center;
        }
        .add-to-cart button {
            padding: 5px 10px;
            background-color: #007BFF;
            color: white;
            border: none;
            cursor: pointer;
        }
        .add-to-cart button:hover {
            background-color: #0056b3;
        }
    </style>
    <script>
        let currentPage = 1;
        let currentCategory = null;

        // Fetch categories from the server
        function fetchCategories() {
            fetch('../ajax/fetch_categories.php')
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        const categoryDropdown = document.getElementById('category');
                        categoryDropdown.innerHTML = '<option value="">All Categories</option>'; // Default option

                        data.categories.forEach(category => {
                            const option = document.createElement('option');
                            option.value = category.id;
                            option.textContent = category.name;
                            categoryDropdown.appendChild(option);
                        });
                    } else {
                        alert('Error: ' + data.message);
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Fetch products from the server
        function fetchProducts() {
            const url = `../ajax/fetch_products.php?category=${currentCategory || ''}&page=${currentPage}`;
            fetch(url)
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        const productsContainer = document.getElementById('products');
                        productsContainer.innerHTML = ''; // Clear existing products

                        // Display products
                        data.products.forEach(product => {
                            const productDiv = document.createElement('div');
                            productDiv.classList.add('product');

                            productDiv.innerHTML = `
                                <div class="product-details">
                                    <h2>${product.product_name}</h2>
                                    <p>Price: $${product.price}</p>
                                    <p>Category: ${product.category_name}</p>
                                    <p>${product.description}</p>
                                    <div class="add-to-cart">
                                        <label for="quantity-${product.product_id}">Quantity:</label>
                                        <input type="number" id="quantity-${product.product_id}" value="1" min="1" max="99">
                                        <button onclick="addToCart(${product.product_id})">Add to Cart</button>
                                    </div>
                                </div>
                                <img src="${product.image_url}" alt="${product.product_name}">
                            `;

                            productsContainer.appendChild(productDiv);
                        });

                        // Handle pagination buttons
                        const previousButton = document.getElementById('previousButton');
                        const nextButton = document.getElementById('nextButton');

                        // Show "Previous" button only if not on the first page
                        if (currentPage > 1) {
                            previousButton.style.display = 'inline-block';
                        } else {
                            previousButton.style.display = 'none';
                        }

                        // Show "Next" button only if there are more products to show
                        if (data.products.length === 4) { // Assuming 4 products per page
                            nextButton.style.display = 'inline-block';
                        } else {
                            nextButton.style.display = 'none';
                        }
                    } else {
                        alert('Error: ' + data.message);
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Add product to cart
        function addToCart(productId) {
            const quantityInput = document.getElementById(`quantity-${productId}`);
            const quantity = parseInt(quantityInput.value);

            if (quantity < 1 || quantity > 99) {
                alert('Please enter a valid quantity (1-99).');
                return;
            }

            fetch('../ajax/add_to_cart.php', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: productId, quantity: quantity })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert(data.message); // "Product added to cart."
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        // Handle category change
        function onCategoryChange() {
            const categoryDropdown = document.getElementById('category');
            currentCategory = categoryDropdown.value;
            currentPage = 1; // Reset to the first page
            fetchProducts();
        }

        // Handle pagination
        function changePage(direction) {
            currentPage += direction;
            if (currentPage < 1) currentPage = 1; // Prevent going below page 1
            fetchProducts();
        }

        // Fetch categories and products when the page loads
        window.onload = () => {
            fetchCategories();
            fetchProducts();
        };
    </script>
</head>
<body>
    <h1>Browse Products</h1>
        <!-- Navigation Buttons -->
        <div style="margin-bottom: 20px; display: flex; gap: 10px;">
        <!-- View Cart Button -->
        <a href="cart.php" style="
            padding: 10px 20px;
            background-color: #007BFF;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            font-size: 1em;
            display: inline-block;
        ">
            View Cart
        </a>

        <!-- Go to Admin Page Button -->
        <a href="admin.php" style="
            padding: 10px 20px;
            background-color: #28a745;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            font-size: 1em;
            display: inline-block;
        ">
            Go to Admin Page
        </a>
    </div>
    <div>
        <label for="category">Filter by Category:</label>
        <select id="category" onchange="onCategoryChange()"></select>
    </div>
    <div id="products">
        <!-- Products will be dynamically loaded here -->
    </div>
    <div style="margin-top: 20px;">
    <button id="previousButton" onclick="changePage(-1)" style="
        padding: 10px 20px;
        background-color: #007BFF;
        color: white;
        border: none;
        cursor: pointer;
        font-size: 1em;
        border-radius: 5px;
        display: none;">
        Previous
    </button>
    <button id="nextButton" onclick="changePage(1)" style="
        padding: 10px 20px;
        background-color: #007BFF;
        color: white;
        border: none;
        cursor: pointer;
        font-size: 1em;
        border-radius: 5px;
        display: none;">
        Next
    </button>
</div>
</body>
</html>