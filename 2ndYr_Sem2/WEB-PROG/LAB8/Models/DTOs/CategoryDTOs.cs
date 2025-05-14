using System.ComponentModel.DataAnnotations;

namespace LAB8.Models.DTOs;

public class CategoryDto
{
    [Required]
    public required int Id { get; set; }

    [Required]
    [MaxLength(40)]
    public required string Name { get; set; }
}
