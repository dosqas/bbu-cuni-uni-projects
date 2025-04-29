import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ApiService } from '../api.service';
import { Router } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';

@Component({
  selector: 'app-cart',
  standalone: true,
  imports: [CommonModule, FormsModule, HttpClientModule],
  templateUrl: './cart.component.html',
  styleUrls: ['./cart.component.css'],
})
export class CartComponent implements OnInit {
  cart: any[] = [];
  errorMessage: string | null = null;

  constructor(private apiService: ApiService, private router: Router) {}

  ngOnInit(): void {
    this.fetchCart();
  }

  // Fetch cart items from the server
  fetchCart(): void {
    this.apiService.fetchCart().subscribe(data => {
      if (data.success) {
        this.cart = data.cart;
      } else {
        this.errorMessage = 'Error: ' + data.message;
      }
    });
  }

  // Update cart item quantity
  updateCart(productId: number, quantity: number): void {
    this.apiService.updateCart({ id: productId, quantity: quantity }).subscribe(data => {
      if (data.success) {
        alert(data.message); // "Cart updated successfully"
        this.fetchCart(); // Refresh the cart
      } else {
        alert('Error: ' + data.message);
      }
    });
  }

  // Remove item from cart
  removeFromCart(productId: number): void {
    this.apiService.removeFromCart(productId).subscribe(data => {
      if (data.success) {
        alert(data.message); // "Item removed from cart."
        this.fetchCart(); // Refresh the cart
      } else {
        alert('Error: ' + data.message);
      }
    });
  }

  // Navigate to the browse page
  goToBrowsePage(): void {
    this.router.navigate(['/browse']);
  }
}
