using LAB8.ViewModels;

namespace LAB8.Services.Interfaces;

public interface ICartService
{
    Task<List<CartItemViewModel>> GetCartAsync(string userId);
    Task<bool> AddToCartAsync(string userId, int productId, int quantity);
    Task<bool> UpdateQuantityAsync(string userId, int productId, int quantity);
    Task<bool> RemoveFromCartAsync(string userId, int productId);
}

