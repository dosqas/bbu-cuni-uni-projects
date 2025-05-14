using Microsoft.AspNetCore.Mvc;
using LAB8.Services.Interfaces;
using LAB8.Models.DTOs;
using LAB8.Utils;
using Microsoft.AspNetCore.Authorization;

namespace LAB8.Controllers;

[ApiController]
[Authorize]
[Route("api/[controller]")]
public class CartController(ICartService cartService) : ControllerBase
{
    private readonly ICartService _cartService = cartService;

    [HttpGet]
    public async Task<IActionResult> GetCart()
    {
        var userId = UserUtils.GetUserId(User);
        var result = await _cartService.GetCartAsync(userId);
        return Ok(new { success = true, cart = result });
    }

    [HttpPost]
    public async Task<IActionResult> AddToCart([FromBody] CartItemCreateDto dto)
    {
        if (!ModelState.IsValid)
            return BadRequest(new { success = false, message = "Invalid input.", errors = ModelState });

        var userId = UserUtils.GetUserId(User);
        var success = await _cartService.AddToCartAsync(userId, dto.ProductId, dto.Quantity);

        return success
            ? Ok(new { success = true, message = "Product added to cart." })
            : BadRequest(new { success = false, message = "Add to cart failed." });
    }

    [HttpPatch("{productId}")]
    public async Task<IActionResult> UpdateCartItem(int productId, [FromBody] CartItemUpdateDto dto)
    {
        if (!ModelState.IsValid)
            return BadRequest(new { success = false, message = "Invalid input.", errors = ModelState });

        var userId = UserUtils.GetUserId(User);
        var success = await _cartService.UpdateQuantityAsync(userId, productId, dto.Quantity);

        return success
            ? Ok(new { success = true, message = "Cart updated successfully." })
            : BadRequest(new { success = false, message = "Update failed." });
    }

    [HttpDelete("{productId}")]
    public async Task<IActionResult> RemoveFromCart(int productId)
    {
        var userId = UserUtils.GetUserId(User);
        var success = await _cartService.RemoveFromCartAsync(userId, productId);

        return success
            ? Ok(new { success = true, message = "Product removed from cart." })
            : BadRequest(new { success = false, message = "Remove failed." });
    }
}
