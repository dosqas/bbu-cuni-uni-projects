import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ApiService } from '../api.service';
import { Router } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';  

@Component({
  selector: 'app-admin',
  standalone: true,
  imports: [CommonModule, FormsModule, HttpClientModule], 
  templateUrl: './admin.component.html',
  styleUrls: ['./admin.component.css'],
})
export class AdminComponent implements OnInit {
  products: any[] = [];
  categories: any[] = [];
  newProduct = { name: '', price: 0, category: 0, description: '' };
  editProductData: any = {};
  isEditModalOpen = false;
  currentPage = 1;
  hasMoreProducts = false;

  constructor(private apiService: ApiService, private router: Router) {}

  ngOnInit(): void {
    this.fetchProducts();
    this.fetchCategories();
  }

  fetchProducts(): void {
    this.apiService.fetchProducts(null, this.currentPage).subscribe({
      next: (response) => {
        if (response.success) {
          this.products = response.products;
          this.hasMoreProducts = response.hasMore;
        } else {
          // maybe navigate away
          console.error('Fetch products failed');
          this.router.navigate(['/browse']);
        }
      },
      error: (error) => {
        console.error('Error fetching products', error);
        this.router.navigate(['/browse']);
      }
    });
  }

  fetchCategories(): void {
    this.apiService.fetchCategories().subscribe((response) => {
      if (response.success) {
        this.categories = response.categories;
      }
    });
  }

  addProduct(): void {
    this.apiService.addProduct(this.newProduct).subscribe((response) => {
      if (response.success) {
        alert('Product added successfully!');
        this.fetchProducts();
        this.newProduct = { name: '', price: 0, category: 0, description: '' };
      }
    });
  }

  deleteProduct(productId: number): void {
    if (confirm('Are you sure you want to delete this product?')) {
      this.apiService.deleteProduct(productId).subscribe((response) => {
        if (response.success) {
          alert('Product deleted successfully!');
          this.fetchProducts();
        }
      });
    }
  }

  editProduct(product: any): void {
    // Find the category ID based on the category name
    const category = this.categories.find((cat) => cat.name === product.category_name);
  
    this.editProductData = {
      id: product.product_id, // Map product_id to id
      name: product.product_name, // Map product_name to name
      price: product.price,
      category: category ? category.id : null, // Map category_name to category ID
      description: product.description,
    };
  
    this.isEditModalOpen = true;
  }

  updateProduct(): void {
    this.apiService.editProduct(this.editProductData).subscribe((response) => {
      if (response.success) {
        alert('Product updated successfully!');
        this.fetchProducts();
        this.closeEditModal();
      }
    });
  }

  closeEditModal(): void {
    this.isEditModalOpen = false;
  }

  changePage(direction: number): void {
    this.currentPage += direction;
    this.fetchProducts();
  }

  goToBrowsePage(): void {
    this.router.navigate(['/browse']);
  }
}
