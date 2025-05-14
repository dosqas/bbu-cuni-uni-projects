using Microsoft.EntityFrameworkCore;
using LAB8.Models;

namespace LAB8.Data;

public class AppDbContext(DbContextOptions<AppDbContext> options) : DbContext(options)
{
    public DbSet<Category> Categories => Set<Category>();
    public DbSet<Product> Products => Set<Product>();
    public DbSet<User> Users => Set<User>();
    public DbSet<Cart> Carts => Set<Cart>();
    public DbSet<CartItem> CartItems => Set<CartItem>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        modelBuilder.Entity<Category>().HasData(
            new Category { Id = 1, Name = "Electronics" },
            new Category { Id = 2, Name = "Clothing" },
            new Category { Id = 3, Name = "Home Appliances" },
            new Category { Id = 4, Name = "Books" },
            new Category { Id = 5, Name = "Toys" }
        );

        modelBuilder.Entity<Product>().HasData(
            new Product { Id = 1, Name = "Smartphone", CategoryId = 1, Price = 499.99m, Description = "Sample description" },
            new Product { Id = 2, Name = "Laptop", CategoryId = 1, Price = 899.99m, Description = "Sample description" },
            new Product { Id = 3, Name = "T-shirt", CategoryId = 2, Price = 19.99m, Description = "Sample description" },
            new Product { Id = 4, Name = "Refrigerator", CategoryId = 3, Price = 399.99m, Description = "Sample description" },
            new Product { Id = 5, Name = "Novel Book", CategoryId = 4, Price = 14.99m, Description = "Sample description" },
            new Product { Id = 6, Name = "Toy Car", CategoryId = 5, Price = 9.99m, Description = "Sample description" }
        );

        modelBuilder.Entity<User>().HasData(
            new User { Id = "1", Username = "admin", Password = "admin"}
        );

        modelBuilder.Entity<User>().HasData(
            new User { Id = "2", Username = "admin1", Password = "admin"}
        );

        modelBuilder.Entity<Cart>().HasData(
            new Cart { Id = 1, UserId = "1" }
        );

        modelBuilder.Entity<Cart>().HasData(
            new Cart { Id = 2, UserId = "2" }
        );

        modelBuilder.Entity<CartItem>().HasData(
            new CartItem { Id = 1, CartId = 1, ProductId = 1, Quantity = 1 }
        );

        modelBuilder.Entity<CartItem>().HasData(
            new CartItem { Id = 2, CartId = 2, ProductId = 2, Quantity = 1 }
        );

        // Fix Cart-User relationship (Cart has one User)
        modelBuilder.Entity<Cart>()
            .HasOne(c => c.User)
            .WithMany() // User can have many carts if needed, or "WithOne" if one-to-one
            .HasForeignKey(c => c.UserId)
            .OnDelete(DeleteBehavior.Cascade); // Optional: Adjust delete behavior based on needs

        // CartItem to Cart relationship (CartItem belongs to one Cart)
        modelBuilder.Entity<CartItem>()
            .HasOne(ci => ci.Cart)
            .WithMany(c => c.Items)
            .HasForeignKey(ci => ci.CartId)
            .OnDelete(DeleteBehavior.Cascade);

        // CartItem to Product relationship (CartItem belongs to one Product)
        modelBuilder.Entity<CartItem>()
            .HasOne(ci => ci.Product)
            .WithMany()
            .HasForeignKey(ci => ci.ProductId)
            .OnDelete(DeleteBehavior.Cascade);

        // Product to Category relationship (Product belongs to one Category)
        modelBuilder.Entity<Product>()
            .HasOne(p => p.Category)
            .WithMany(c => c.Products)
            .HasForeignKey(p => p.CategoryId)
            .OnDelete(DeleteBehavior.Cascade);
    }
}
