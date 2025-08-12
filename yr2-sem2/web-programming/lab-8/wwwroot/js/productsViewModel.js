class ProductView {
    constructor() {
        this.currentPage = 1;
        this.currentCategory = null;
        this.pageSize = 4;
    }

    async fetchCategories() {
        try {
            const response = await fetch('/api/categories');
            const data = await response.json();
            
            if (data.success) {
                const categoryDropdown = document.getElementById('category');
                categoryDropdown.innerHTML = '<option value="">All Categories</option>';

                data.categories.forEach(category => {
                    const option = document.createElement('option');
                    option.value = category.id;
                    option.textContent = category.name;
                    categoryDropdown.appendChild(option);
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
            let url = `/api/products?page=${this.currentPage}`;
            if (this.currentCategory) {
                url += `&category=${this.currentCategory}`;
            }

            const response = await fetch(url);
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
        const productsContainer = document.getElementById('products');
        productsContainer.innerHTML = '';

        if (products.length === 0) {
            productsContainer.innerHTML = '<p>No products found.</p>';
            return;
        }

        products.forEach(product => {
            const productDiv = document.createElement('div');
            productDiv.classList.add('product');

            productDiv.innerHTML = `
                <div class="product-details">
                    <h2>${product.name}</h2>
                    <p>Price: $${product.price}</p>
                    <p>Category: ${product.categoryName}</p>
                    <p>${product.description}</p>
                    <div class="add-to-cart">
                        <label for="quantity-${product.id}">Quantity:</label>
                        <input type="number" id="quantity-${product.id}" value="1" min="1" max="99">
                        <button onclick="productView.addToCart(${product.id})" class="btn btn-primary">Add to Cart</button>
                    </div>
                </div>
            `;

            productsContainer.appendChild(productDiv);
        });

        // Handle pagination buttons
        const previousButton = document.getElementById('previousButton');
        const nextButton = document.getElementById('nextButton');

        previousButton.style.display = this.currentPage > 1 ? 'inline-block' : 'none';
        nextButton.style.display = hasMore ? 'inline-block' : 'none';
    }

    async addToCart(productId) {
        try {
            const quantityInput = document.getElementById(`quantity-${productId}`);
            const quantity = parseInt(quantityInput.value);

            if (quantity < 1 || quantity > 99) {
                alert('Please enter a valid quantity (1-99).');
                return;
            }

            const response = await fetch('/api/cart', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ productId, quantity })
            });
            
            const data = await response.json();
            
            if (data.success) {
                alert('Product added to cart successfully!');
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error adding to cart:', error);
            alert('Failed to add product to cart.');
        }
    }

    onCategoryChange() {
        const categoryDropdown = document.getElementById('category');
        this.currentCategory = categoryDropdown.value || null;
        this.currentPage = 1;
        this.fetchProducts();
    }

    changePage(direction) {
        this.currentPage += direction;
        if (this.currentPage < 1) this.currentPage = 1;
        this.fetchProducts();
    }
}

// Initialize the product view
const productView = new ProductView();

// Event listeners
document.addEventListener('DOMContentLoaded', () => {
    productView.fetchCategories();
    productView.fetchProducts();
});

// Global functions for HTML event handlers
window.onCategoryChange = () => productView.onCategoryChange();
window.changePage = (direction) => productView.changePage(direction);
window.addToCart = (productId) => productView.addToCart(productId);