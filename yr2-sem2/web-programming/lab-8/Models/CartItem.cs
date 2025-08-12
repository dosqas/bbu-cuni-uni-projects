using System.ComponentModel.DataAnnotations;

namespace LAB8.Models;

public class CartItem
{
    public int Id { get; set; }

    [Required]
    public int ProductId { get; set; }

    [Required]
    [Range(1, int.MaxValue, ErrorMessage = "Quantity must be at least 1.")]
    public int Quantity { get; set; }

    [Required]
    public int CartId { get; set; }

    [Required]
    public Cart Cart { get; set; } = null!;

    [Required]
    public Product Product { get; set; } = null!;
}
