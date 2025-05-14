class AdminView {
    constructor() {
        this.currentPage = 1;
        this.currentEditProductId = null;
    }

    async initialize() {
        await this.fetchCategories();
        await this.fetchProducts();
    }

    async fetchCategories() {
        try {
            const response = await fetch('/api/categories');
            const data = await response.json();
            
            if (data.success) {
                // Populate both dropdowns
                ['product-category', 'edit-product-category'].forEach(id => {
                    const dropdown = document.getElementById(id);
                    if (dropdown) {
                        dropdown.innerHTML = '<option value="">Select a Category</option>';
                        data.categories.forEach(category => {
                            const option = document.createElement('option');
                            option.value = category.id;
                            option.textContent = category.name;
                            dropdown.appendChild(option);
                        });
                    }
                });
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error fetching categories:', error);
        }
    }

    async fetchProducts() {
        try {
            const response = await fetch(`/api/products?page=${this.currentPage}`);
            const data = await response.json();
            
            if (data.success) {
                this.renderProducts(data.products, data.hasMore);
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error fetching products:', error);
        }
    }

    renderProducts(products, hasMore) {
        const productList = document.getElementById('product-list');
        productList.innerHTML = '';

        if (products.length === 0) {
            productList.innerHTML = '<p>No products found.</p>';
            return;
        }

        products.forEach(product => {
            const productDiv = document.createElement('div');
            productDiv.classList.add('product-item');

            productDiv.innerHTML = `
                <h3>${product.name}</h3>
                <p>Price: $${product.price}</p>
                <p>Category: ${product.categoryName}</p>
                <p>${product.description}</p>
                <button onclick="adminView.editProduct(${product.id})" class="btn btn-primary">Edit</button>
                <button onclick="adminView.deleteProduct(${product.id})" class="btn btn-danger">Delete</button>
            `;

            productList.appendChild(productDiv);
        });

        // Handle pagination buttons
        const previousButton = document.getElementById('previousButton');
        const nextButton = document.getElementById('nextButton');

        previousButton.style.display = this.currentPage > 1 ? 'inline-block' : 'none';
        nextButton.style.display = hasMore ? 'inline-block' : 'none';
    }

    async addProduct() {
        const name = document.getElementById('product-name').value.trim();
        const price = parseFloat(document.getElementById('product-price').value.trim());
        const categoryId = parseInt(document.getElementById('product-category').value.trim());
        const description = document.getElementById('product-description').value.trim();

        // Validation
        if (!this.validateProductInput(name, price, categoryId, description)) {
            return;
        }

        try {
            const response = await fetch('/api/products', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    name: name,
                    price: price,
                    categoryId: categoryId,
                    description: description
                })
            });
            
            const data = await response.json();
            
            if (data.success) {
                alert('Product added successfully!');
                this.clearAddForm();
                this.fetchProducts();
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error adding product:', error);
            alert('Failed to add product.');
        }
    }

    clearAddForm() {
        document.getElementById('product-name').value = '';
        document.getElementById('product-price').value = '';
        document.getElementById('product-category').value = '';
        document.getElementById('product-description').value = '';
    }

    async deleteProduct(productId) {
        if (!confirm('Are you sure you want to delete this product?')) return;

        try {
            const response = await fetch(`/api/products/${productId}`, {
                method: 'DELETE'
            });
            
            const data = await response.json();
            
            if (data.success) {
                alert('Product deleted successfully!');
                this.fetchProducts();
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error deleting product:', error);
            alert('Failed to delete product.');
        }
    }

    async editProduct(productId) {
        this.currentEditProductId = productId;

        try {
            // First fetch categories to ensure dropdown is populated
            await this.fetchCategories();
            
            // Then fetch the product data
            const response = await fetch(`/api/products/${productId}`);
            const data = await response.json();
            
            if (data.success) {
                const product = data.product;
                document.getElementById('edit-product-name').value = product.name;
                document.getElementById('edit-product-price').value = product.price;
                document.getElementById('edit-product-category').value = product.categoryId;
                document.getElementById('edit-product-description').value = product.description;
                
                // Show the modal
                document.getElementById('edit-product-modal').style.display = 'block';
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error editing product:', error);
        }
    }

    closeEditModal() {
        document.getElementById('edit-product-modal').style.display = 'none';
        this.currentEditProductId = null;
    }

    async updateProduct() {
        const name = document.getElementById('edit-product-name').value.trim();
        const price = parseFloat(document.getElementById('edit-product-price').value.trim());
        const categoryId = parseInt(document.getElementById('edit-product-category').value.trim());
        const description = document.getElementById('edit-product-description').value.trim();

        // Validation
        if (!this.validateProductInput(name, price, categoryId, description)) {
            return;
        }

        try {
            const response = await fetch(`/api/products/${this.currentEditProductId}`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    name: name,
                    price: price,
                    categoryId: categoryId,
                    description: description
                })
            });
            
            const data = await response.json();
            
            if (data.success) {
                alert('Product updated successfully!');
                this.closeEditModal();
                this.fetchProducts();
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error updating product:', error);
            alert('Failed to update product.');
        }
    }

    validateProductInput(name, price, categoryId, description) {
        if (!name || name.length > 40) {
            alert('Product name must be between 1 and 40 characters.');
            return false;
        }

        if (isNaN(price)) {
            alert('Price must be a valid number.');
            return false;
        }

        if (!categoryId) {
            alert('Please select a category.');
            return false;
        }

        if (description.length > 200) {
            alert('Description must not exceed 200 characters.');
            return false;
        }

        return true;
    }

    changePage(direction) {
        this.currentPage += direction;
        if (this.currentPage < 1) this.currentPage = 1;
        this.fetchProducts();
    }
}

// Initialize the admin view
const adminView = new AdminView();

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    adminView.initialize();
});