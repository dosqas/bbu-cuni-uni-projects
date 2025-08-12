using LAB8.Data;
using LAB8.Models.DTOs;
using LAB8.Services.Interfaces;
using Microsoft.EntityFrameworkCore;

namespace LAB8.Services;

public class CategoryService(AppDbContext context) : ICategoryService
{
    private readonly AppDbContext _context = context;

    public async Task<List<CategoryDto>> GetAllCategoriesAsync()
    {
        try
        {
            return await _context.Categories
                .Select(c => new CategoryDto
                {
                    Id = c.Id,
                    Name = c.Name
                })
                .ToListAsync();
        }
        catch (Exception ex)
        {
            throw new Exception($"Database error: {ex.Message}");
        }
    }
}
