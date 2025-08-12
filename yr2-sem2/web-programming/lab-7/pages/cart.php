<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Your Cart</title>
    <style>
        .cart-item {
            border: 1px solid #ccc;
            padding: 10px;
            margin-bottom: 10px;
            max-width: 400px;
        }
        .cart-item h2 {
            margin: 0;
        }
        .cart-item p {
            margin: 5px 0;
        }
        .cart-item input {
            width: 50px;
            text-align: center;
        }
        .cart-item button {
            padding: 5px 10px;
            background-color: #FF0000;
            color: white;
            border: none;
            cursor: pointer;
        }
        .cart-item button:hover {
            background-color: #CC0000;
        }
    </style>
    <script>
        // Fetch cart items from the server
        function fetchCart() {
            fetch('../ajax/fetch_cart.php')
                .then(response => response.json())
                .then(data => {
                    if (data.success) {
                        const cartContainer = document.getElementById('cart');
                        cartContainer.innerHTML = ''; // Clear existing cart items

                        if (data.cart.length > 0) {
                            data.cart.forEach(item => {
                                const cartItemDiv = document.createElement('div');
                                cartItemDiv.classList.add('cart-item');

                                cartItemDiv.innerHTML = `
                                    <h2>${item.name}</h2>
                                    <p>Price: $${item.price}</p>
                                    <p>Quantity: 
                                        <input type="number" value="${item.quantity}" min="1" 
                                               onchange="updateCart(${item.product_id}, this.value)">
                                    </p>
                                    <button onclick="removeFromCart(${item.product_id})">Remove</button>
                                `;

                                cartContainer.appendChild(cartItemDiv);
                            });
                        } else {
                            cartContainer.innerHTML = '<p>Your cart is empty.</p>';
                        }
                    } else {
                        alert('Error: ' + data.message);
                    }
                })
                .catch(error => console.error('Error:', error));
        }

        // Update cart item quantity
        function updateCart(productId, quantity) {
            fetch('../ajax/update_cart.php', {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: productId, quantity: quantity })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert(data.message); // "Cart updated successfully."
                    fetchCart(); // Refresh the cart
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        // Remove item from cart
        function removeFromCart(productId) {
            fetch('../ajax/remove_from_cart.php', {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ id: productId })
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    alert(data.message); // "Product removed from cart."
                    fetchCart(); // Refresh the cart
                } else {
                    alert('Error: ' + data.message);
                }
            })
            .catch(error => console.error('Error:', error));
        }

        // Fetch cart items when the page loads
        window.onload = fetchCart;
    </script>
</head>
<body>
    <h1>Your Cart</h1>
    <a href="browse.php">Back to Browse</a>
    <div id="cart">
        <!-- Cart items will be dynamically loaded here -->
    </div>
</body>
</html>