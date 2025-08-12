using LAB8.Models.DTOs;

namespace LAB8.Services.Interfaces;

public interface ICategoryService
{
    Task<List<CategoryDto>> GetAllCategoriesAsync();
}
