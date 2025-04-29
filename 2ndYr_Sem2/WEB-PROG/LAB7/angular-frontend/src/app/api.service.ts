import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class ApiService {
  private baseUrl = 'http://localhost:8000/ajax'; 

  constructor(private http: HttpClient) {}

  // Add Product
  addProduct(product: { name: string; price: number; category: number; description: string }): Observable<any> {
    return this.http.post(`${this.baseUrl}/add_product.php`, product);
  }

  // Add To Cart
  addToCart(cartItem: { id: number; quantity: number }): Observable<any> {
    return this.http.post(`${this.baseUrl}/add_to_cart.php`, cartItem);
  }

  // Delete Product
  deleteProduct(productId: number): Observable<any> {
    return this.http.request('delete', `${this.baseUrl}/delete_product.php`, {
      body: { id: productId },
    });
  }

  // Edit Product
  editProduct(product: { id: number; name: string; price: number; category: number; description: string }): Observable<any> {
    return this.http.put(`${this.baseUrl}/edit_product.php`, product);
  }

  // Fetch Cart
  fetchCart(): Observable<any> {
    return this.http.get(`${this.baseUrl}/fetch_cart.php`);
  }

  // Fetch Categories
  fetchCategories(): Observable<any> {
    return this.http.get(`${this.baseUrl}/fetch_categories.php`);
  }

  // Fetch Product
  fetchProductById(productId: number): Observable<any> {
    const params = { id: productId.toString() };
    return this.http.get(`${this.baseUrl}/fetch_product.php`, { params });
  }

  // Fetch Products
  fetchProducts(category: string | null, page: number): Observable<any> {
    const params = {
      category: category ? category.toString() : '',
      page: page.toString(),
    };
    return this.http.get(`${this.baseUrl}/fetch_products.php`, { params });
  }

  // Remove From Cart
  removeFromCart(productId: number): Observable<any> {
    return this.http.request('delete', `${this.baseUrl}/remove_from_cart.php`, {
      body: { id: productId },
    });
  }

  // Update Cart
  updateCart(cartItem: { id: number; quantity: number }): Observable<any> {
    return this.http.patch(`${this.baseUrl}/update_cart.php`, cartItem);
  }
}