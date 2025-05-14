using Microsoft.AspNetCore.Mvc;
using YourProject.Models; // For CartItem model
using YourProject.Services; // For cart service

public class CartController : Controller
{
    private readonly ICartService _cartService;

    public CartController(ICartService cartService)
    {
        _cartService = cartService;
    }

    public IActionResult Index()
    {
        var userId = HttpContext.Session.GetInt32("UserId");
        if (userId == null)
            return RedirectToAction("Login", "Account");

        var cartItems = _cartService.GetCartItemsByUserId(userId.Value);
        return View(cartItems);
    }
}
