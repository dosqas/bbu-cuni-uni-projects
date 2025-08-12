import { Routes } from '@angular/router';

// Import the components
import { AdminComponent } from './admin/admin.component';
import { CartComponent } from './cart/cart.component';
import { BrowseComponent } from './browse/browse.component';

// Define the routes
export const routes: Routes = [
  { path: 'admin', component: AdminComponent },
  { path: 'cart', component: CartComponent },
  { path: 'browse', component: BrowseComponent },
  { path: '', redirectTo: '/browse', pathMatch: 'full' },  // Default route (redirects to browse)
  { path: '**', redirectTo: '/browse' }  // Wildcard route for invalid URLs
];
