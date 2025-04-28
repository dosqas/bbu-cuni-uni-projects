<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Admin Panel</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        .form-container {
            margin-bottom: 20px;
            padding: 10px;
            border: 1px solid #ccc;
            max-width: 600px;
        }
        .form-container h2 {
            margin-top: 0;
        }
        .form-container label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
        }
        .form-container input, .form-container select, .form-container textarea, .form-container button {
            width: 95%;
            padding: 8px;
            margin-bottom: 10px;
            font-size: 1em;
        }
        .form-container button {
            background-color: #007BFF;
            color: white;
            border: none;
            cursor: pointer;
        }
        .form-container button:hover {
            background-color: #0056b3;
        }
        .product-list, .category-list {
            margin-top: 20px;
        }
        .product-item, .category-item {
            border: 1px solid #ccc;
            padding: 10px;
            margin-bottom: 10px;
        }
        .product-item button, .category-item button {
            margin-right: 10px;
        }
        #edit-product-modal {
            width: 400px;
            background-color: #fff;
            border-radius: 8px;
        }
        #edit-product-modal button {
            margin-top: 10px;
        }
    </style>
    <script>
        // Fetch products and categories when the page loads
        window.onload = () => {
            fetchProducts();
            fetchCategories();
        };

        // Fetch products
        function fetchProducts() {
            fetch(`../ajax/fetch_products.php?page=${currentPage}`)
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        const productList = document.getElementById('product-list');
                        productList.innerHTML = ''; // Clear existing products

                        // Display products
                        data.products.forEach(product => {
                            const productDiv = document.createElement('div');
                            productDiv.classList.add('product-item');

                            productDiv.innerHTML = `
                                <h3>${product.product_name}</h3>
                                <p>Price: $${product.price}</p>
                                <p>Category: ${product.category_name}</p>
                                <p>${product.description}</p>
                                <button onclick="editProduct(${product.product_id})">Edit</button>
                                <button onclick="deleteProduct(${product.product_id})">Delete</button>
                            `;

                            productList.appendChild(productDiv);
                        });

                        // Handle pagination buttons
                        const previousButton = document.getElementById('previousButton');
                        const nextButton = document.getElementById('nextButton');

                        // Show "Previous" button only if not on the first page
                        previousButton.style.display = currentPage > 1 ? 'inline-block' : 'none';

                        // Show "Next" button only if there are more products to show
                        nextButton.style.display = data.hasMore ? 'inline-block' : 'none';
                    } else {
                        alert('Error: ' + data.message);
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Fetch categories
        function fetchCategories() {
            console.log("Fetching categories..."); // Debugging: Log when the function is called

            fetch('../ajax/fetch_categories.php')
                .then(response => response.json())
                .then(data => {
                    console.log("Categories response:", data); // Debugging: Log the response

                    if (data.success) {
                        // Populate the Add Product dropdown
                        const categoryDropdown = document.getElementById('product-category');
                        if (categoryDropdown) {
                            categoryDropdown.innerHTML = '<option value="">Select a Category</option>';
                            data.categories.forEach(category => {
                                const option = document.createElement('option');
                                option.value = category.id;
                                option.textContent = category.name;
                                categoryDropdown.appendChild(option);
                            });
                            console.log("Add Product dropdown updated."); // Debugging
                        } else {
                            console.error("Add Product dropdown not found."); // Debugging
                        }

                        // Populate the Edit Product dropdown (if it exists)
                        const editCategoryDropdown = document.getElementById('edit-product-category');
                        if (editCategoryDropdown) {
                            editCategoryDropdown.innerHTML = '<option value="">Select a Category</option>';
                            data.categories.forEach(category => {
                                const option = document.createElement('option');
                                option.value = category.id;
                                option.textContent = category.name;
                                editCategoryDropdown.appendChild(option);
                            });
                            console.log("Edit Product dropdown updated."); // Debugging
                        }
                    } else {
                        alert('Error fetching categories: ' + data.message);
                    }
                })
                .catch(error => {
                    console.error('Error fetching categories:', error); // Debugging: Log any errors
                });
        }

        // Add product
        function addProduct() {
            const name = document.getElementById('product-name').value.trim();
            const price = document.getElementById('product-price').value.trim();
            const category = document.getElementById('product-category').value.trim();
            const description = document.getElementById('product-description').value.trim();

            // Validate name (max 40 characters)
            if (name.length === 0 || name.length > 40) {
                alert('Product name must be between 1 and 40 characters.');
                return;
            }

            // Validate price (must be a valid number)
            if (!/^\d+(\.\d{1,2})?$/.test(price)) {
                alert('Price must be a valid number (e.g., 10, 10.99).');
                return;
            }

            // Validate category (must be selected)
            if (!category) {
                alert('Please select a category.');
                return;
            }

            // Validate description (max 200 characters)
            if (description.length > 200) {
                alert('Description must not exceed 200 characters.');
                return;
            }

            // Send the request if validation passes
            fetch('../ajax/add_product.php', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ name, price, category, description })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert('Product added successfully!');
                    fetchProducts(); // Refresh the product list
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        // Delete product
        function deleteProduct(productId) {
            if (!confirm('Are you sure you want to delete this product?')) return;

            fetch('../ajax/delete_product.php', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: productId })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert('Product deleted successfully!');
                    fetchProducts(); // Refresh the product list
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        let currentEditProductId = null;

        // Open the edit modal and populate fields
        function editProduct(productId) {
            currentEditProductId = productId;

            // Fetch categories first to populate the dropdown
            fetch('../ajax/fetch_categories.php')
                .then(response => response.json())
                .then(categoryData => {
                    if (categoryData.success) {
                        // Populate the Edit Product dropdown
                        const editCategoryDropdown = document.getElementById('edit-product-category');
                        editCategoryDropdown.innerHTML = '<option value="">Select a Category</option>';
                        categoryData.categories.forEach(category => {
                            const option = document.createElement('option');
                            option.value = category.id;
                            option.textContent = category.name;
                            editCategoryDropdown.appendChild(option);
                        });

                        // Fetch product data
                        return fetch(`../ajax/fetch_product.php?id=${productId}`);
                    } else {
                        throw new Error('Error fetching categories: ' + categoryData.message);
                    }
                })
                .then(response => response.json())
                .then(productData => {
                    if (productData.success) {
                        // Populate the modal fields with product data
                        document.getElementById('edit-product-name').value = productData.product.name;
                        document.getElementById('edit-product-price').value = productData.product.price;
                        document.getElementById('edit-product-category').value = productData.product.category_id;
                        document.getElementById('edit-product-description').value = productData.product.description;

                        // Show the modal
                        document.getElementById('edit-product-modal').style.display = 'block';
                    } else {
                        alert('Error fetching product: ' + productData.message);
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Close the edit modal
        function closeEditModal() {
            document.getElementById('edit-product-modal').style.display = 'none';
        }

        // Update the product
        function updateProduct() {
            const name = document.getElementById('edit-product-name').value.trim();
            const price = document.getElementById('edit-product-price').value.trim();
            const category = document.getElementById('edit-product-category').value.trim();
            const description = document.getElementById('edit-product-description').value.trim();

            // Validate name (max 40 characters)
            if (name.length === 0 || name.length > 40) {
                alert('Product name must be between 1 and 40 characters.');
                return;
            }

            // Validate price (must be a valid number)
            if (!/^\d+(\.\d{1,2})?$/.test(price)) {
                alert('Price must be a valid number (e.g., 10, 10.99).');
                return;
            }

            // Validate category (must be selected)
            if (!category) {
                alert('Please select a category.');
                return;
            }

            // Validate description (max 200 characters)
            if (description.length > 200) {
                alert('Description must not exceed 200 characters.');
                return;
            }

            // Send the update request
            fetch('../ajax/edit_product.php', {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: currentEditProductId, name, price, category, description })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert('Product updated successfully!');
                    fetchProducts(); // Refresh the product list
                    closeEditModal(); // Close the modal
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        function goToBrowsePage() {
            window.location.href = '../pages/browse.php';
        }

        let currentPage = 1; // Start on the first page

        function changePage(direction) {
            currentPage += direction;
            if (currentPage < 1) currentPage = 1; // Prevent going below page 1
            fetchProducts(); // Fetch products for the new page
        }
    </script>
</head>
<body>
    <h1>Admin Panel</h1>
    <div style="margin-bottom: 20px;">
        <button onclick="goToBrowsePage()" style="padding: 10px 20px; background-color: #28a745; color: white; border: none; cursor: pointer; font-size: 1em;">
            Go to Browse Page
        </button>
    </div>

    <!-- Add Product Form -->
    <div class="form-container">
        <h2>Add Product</h2>
        <label for="product-name">Name:</label>
        <input type="text" id="product-name" required>
        <label for="product-price">Price:</label>
        <input type="number" id="product-price" step="0.01" required>
        <label for="product-category">Category:</label>
        <select id="product-category"></select>
        <label for="product-description">Description:</label>
        <textarea id="product-description" rows="4"></textarea>
        <button onclick="addProduct()">Add Product</button>
    </div>

    <!-- Product List -->
    <div id="product-list" class="product-list">
        <!-- Products will be dynamically loaded here -->
    </div>

    <!-- Edit Product Modal -->
    <div id="edit-product-modal" class="form-container" style="display: none; position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%); z-index: 1000; background: white; border: 1px solid #ccc; padding: 20px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);">
        <h2>Edit Product</h2>
        <label for="edit-product-name">Name:</label>
        <input type="text" id="edit-product-name" maxlength="40" required>
        <label for="edit-product-price">Price:</label>
        <input type="number" id="edit-product-price" step="0.01" required>
        <label for="edit-product-category">Category:</label>
        <select id="edit-product-category"></select>
        <label for="edit-product-description">Description:</label>
        <textarea id="edit-product-description" rows="4" maxlength="200"></textarea>
        <button onclick="updateProduct()">Update Product</button>
        <button onclick="closeEditModal()">Cancel</button>
    </div>

    <!-- Pagination Buttons -->
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