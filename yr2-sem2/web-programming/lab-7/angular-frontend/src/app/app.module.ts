import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { provideRouter } from '@angular/router';  // Import provideRouter

// Import the app configuration and routes
import { appConfig } from './app.config';
import { routes } from './app.routes';

// Import components
import { AdminComponent } from './admin/admin.component';
import { CartComponent } from './cart/cart.component';
import { BrowseComponent } from './browse/browse.component';

@NgModule({
  declarations: [
    AdminComponent,
    CartComponent,
    BrowseComponent
  ],
  imports: [
    BrowserModule,
    // provideRouter will be used from the appConfig, so no need to add it here.
  ],
  providers: [],
  bootstrap: [BrowseComponent]  // Use BrowseComponent as the bootstrap component
})
export class AppModule { }
