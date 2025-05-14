using LAB8.Services.Interfaces;
using LAB8.Data;
using LAB8.ViewModels;
using LAB8.Models;
using Microsoft.EntityFrameworkCore;

namespace LAB8.Services;

public class CartService(AppDbContext context) : ICartService
{
    private readonly AppDbContext _context = context;

    public async Task<List<CartItemViewModel>> GetCartAsync(string userId)
    {
        var cart = await _context.Carts
            .Include(c => c.Items)
            .ThenInclude(i => i.Product)
            .FirstOrDefaultAsync(c => c.UserId == userId);

        if (cart == null) return [];

        return [.. cart.Items.Select(item => new CartItemViewModel
        {
            ProductId = item.ProductId,
            Quantity = item.Quantity,
            Name = item.Product.Name,
            Price = item.Product.Price
        })];
    }

    public async Task<bool> AddToCartAsync(string userId, int productId, int quantity)
    {
        if (quantity < 1 || quantity > 99) return false;

        var cart = await _context.Carts
            .Include(c => c.Items)
            .FirstOrDefaultAsync(c => c.UserId == userId);

        if (cart == null)
        {
            cart = new Cart { UserId = userId, Items = new List<CartItem>() };
            _context.Carts.Add(cart);
        }

        var existingItem = cart.Items.FirstOrDefault(i => i.ProductId == productId);
        if (existingItem != null)
        {
            existingItem.Quantity += quantity;
        }
        else
        {
            cart.Items.Add(new CartItem
            {
                ProductId = productId,
                Quantity = quantity
            });
        }

        await _context.SaveChangesAsync();
        return true;
    }

    public async Task<bool> UpdateQuantityAsync(string userId, int productId, int quantity)
    {
        var cart = await _context.Carts
            .Include(c => c.Items)
            .FirstOrDefaultAsync(c => c.UserId == userId);

        if (cart == null) return false;

        var item = cart.Items.FirstOrDefault(i => i.ProductId == productId);
        if (item == null) return false;

        item.Quantity = quantity;
        await _context.SaveChangesAsync();
        return true;
    }

    public async Task<bool> RemoveFromCartAsync(string userId, int productId)
    {
        var cart = await _context.Carts
            .Include(c => c.Items)
            .FirstOrDefaultAsync(c => c.UserId == userId);

        if (cart == null) return false;

        var item = cart.Items.FirstOrDefault(i => i.ProductId == productId);
        if (item == null) return false;

        cart.Items.Remove(item);
        await _context.SaveChangesAsync();
        return true;
    }
}