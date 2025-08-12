class CartViewModel {
    constructor() {
        this.cartItems = [];
    }

    async fetchCart() {
        try {
            const response = await fetch('/api/cart');
            const data = await response.json();
            if (data.success) {
                this.cartItems = data.cart;
                this.renderCart();
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error fetching cart:', error);
        }
    }

    renderCart() {
        const cartContainer = document.getElementById('cart');
        cartContainer.innerHTML = ''; // Clear existing cart items

        if (this.cartItems.length > 0) {
            this.cartItems.forEach(item => {
                console.log('Rendering item:', item);
                const cartItemDiv = document.createElement('div');
                cartItemDiv.classList.add('cart-item');
                cartItemDiv.innerHTML = `
                    <h2>${item.name}</h2>
                    <p>Price: $${item.price}</p>
                    <p>Quantity: 
                        <input type="number" value="${item.quantity}" min="1" 
                               onchange="cartViewModel.updateCart(${item.productId}, this.value)">
                    </p>
                    <button onclick="cartViewModel.removeFromCart(${item.productId})">Remove</button>
                `;
                cartContainer.appendChild(cartItemDiv);
            });
        } else {
            cartContainer.innerHTML = '<p>Your cart is empty.</p>';
        }
    }

    async updateCart(productId, quantity) {
        try {
            const response = await fetch(`/api/cart/${productId}`, {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ quantity })
            });
            const data = await response.json();
            if (data.success) {
                alert(data.message);
                this.fetchCart();  // Refresh cart
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error updating cart:', error);
        }
    }

    async removeFromCart(productId) {
        try {
            const response = await fetch(`/api/cart/${productId}`, {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
            });
            const data = await response.json();
            if (data.success) {
                alert(data.message);
                this.fetchCart();  // Refresh cart
            } else {
                alert('Error: ' + data.message);
            }
        } catch (error) {
            console.error('Error removing from cart:', error);
        }
    }
}

const cartViewModel = new CartViewModel();
window.onload = () => cartViewModel.fetchCart();
