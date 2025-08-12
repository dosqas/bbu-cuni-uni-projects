import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ApiService } from '../api.service';
import { Router } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';  

@Component({
  selector: 'app-browse',
  standalone: true,
  imports: [CommonModule, FormsModule, HttpClientModule], 
  templateUrl: './browse.component.html',
  styleUrls: ['./browse.component.css'],
})
export class BrowseComponent implements OnInit {
  products: any[] = [];
  categories: any[] = [];
  currentPage: number = 1;
  currentCategory: string | null = "";
  hasMoreProducts: boolean = false;

  constructor(private apiService: ApiService, private router: Router) {}

  ngOnInit(): void {
    this.fetchCategories();
    this.fetchProducts();
  }

  // Fetch categories from the server
  fetchCategories(): void {
    this.apiService.fetchCategories().subscribe(data => {
      if (data.success) {
        this.categories = data.categories;
      } else {
        alert('Error: ' + data.message);
      }
    });
  }

  // Fetch products from the server
  fetchProducts(): void {
    this.apiService.fetchProducts(this.currentCategory, this.currentPage).subscribe(data => {
      if (data.success) {
        this.products = data.products.map((product: any) => ({
          ...product,
          quantity: 1, // Default quantity
        }));
        this.hasMoreProducts = data.products.length === 4; // Assuming 4 products per page
      } else {
        alert('Error: ' + data.message);
      }
    });
  }

  // Handle category change
  onCategoryChange(): void {
    const categoryDropdown = document.getElementById('category') as HTMLSelectElement;
    this.currentCategory = categoryDropdown.value;
    this.currentPage = 1;  // Reset to the first page
    this.fetchProducts();
  }

  // Add product to cart
  addToCart(productId: number): void {
    const quantity = (document.getElementById(`quantity-${productId}`) as HTMLInputElement).value;
    this.apiService.addToCart({id: productId, quantity: parseInt(quantity)}).subscribe(data => {
      if (data.success) {
        alert(data.message); // "Product added to cart."
      } else {
        alert('Error: ' + data.message);
      }
    });
  }

  // Handle pagination
  changePage(direction: number): void {
    this.currentPage += direction;
    if (this.currentPage < 1) this.currentPage = 1;  // Prevent going below page 1
    this.fetchProducts();
  }

  goToCartPage() : void {
    this.router.navigate(['/cart']);
  }

  goToAdminPage() : void {
    this.router.navigate(['/admin']);
  }
}
