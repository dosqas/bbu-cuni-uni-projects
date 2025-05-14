using System.ComponentModel.DataAnnotations;

namespace LAB8.Models.DTOs;

public class CartItemCreateDto
{
    [Required]
    public int ProductId { get; set; }

    [Range(1, 100)]
    public int Quantity { get; set; }
}

public class CartItemUpdateDto
{
    [Range(1, 100)]
    public int Quantity { get; set; }
}