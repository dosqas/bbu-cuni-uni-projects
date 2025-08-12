using System.ComponentModel.DataAnnotations;

namespace LAB8.Models;

public class Product
{
    public int Id { get; set; }

    [Required]
    [MaxLength(40)]
    public string Name { get; set; } = string.Empty;

    [Required]
    [Range(0, double.MaxValue, ErrorMessage = "Price must be non-negative.")]
    public decimal Price { get; set; }

    [MaxLength(200)]
    public string? Description { get; set; }

    public int? CategoryId { get; set; }
    public Category? Category { get; set; }
}
