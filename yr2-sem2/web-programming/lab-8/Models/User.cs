using System.ComponentModel.DataAnnotations;

namespace LAB8.Models;

public class User
{
    public string Id { get; set; } = string.Empty;

    [Required]
    [MaxLength(50)]
    public string Username { get; set; } = string.Empty;

    [Required]
    [MaxLength(100)]
    public string Password { get; set; } = string.Empty;

    public ICollection<CartItem> CartItems { get; set; } = [];
}
