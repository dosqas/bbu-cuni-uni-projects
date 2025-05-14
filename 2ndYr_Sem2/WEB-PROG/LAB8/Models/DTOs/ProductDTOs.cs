using System.ComponentModel.DataAnnotations;

namespace LAB8.Models.DTOs;

public class ProductDto
{
    [Required]
    public required int Id { get; set; }

    [Required]
    [MaxLength(40)]
    public required string Name { get; set; }

    [Required]
    [Range(0, double.MaxValue)]
    public required decimal Price { get; set; }

    [MaxLength(200)]
    public required string? Description { get; set; }

    [Required]
    [MaxLength(40)]
    public required string CategoryName { get; set; }
}

public class ProductCreateUpdateDto
{
    [Required]
    [MaxLength(40)]
    public required string Name { get; set; }

    [MaxLength(200)]
    public required string? Description { get; set; }

    [Required]
    [Range(0, double.MaxValue)]
    public required decimal Price { get; set; }

    [Required]
    public required int CategoryId { get; set; }
}