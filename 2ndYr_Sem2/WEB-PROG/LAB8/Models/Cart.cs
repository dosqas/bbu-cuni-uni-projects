using System.ComponentModel.DataAnnotations;

namespace LAB8.Models;

public class Cart
{
    public int Id { get; set; }

    [Required]
    public string UserId { get; set; } = string.Empty;

    public User User { get; set; } = null!;

    public List<CartItem> Items { get; set; } = [];
}
