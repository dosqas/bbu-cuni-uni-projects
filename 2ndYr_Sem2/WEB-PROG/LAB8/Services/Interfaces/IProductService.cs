using LAB8.Models.DTOs;

namespace LAB8.Services.Interfaces;

public interface IProductService
{
    Task<(List<ProductDto> products, bool hasMore)> GetAllAsync(int? categoryId, int page, int pageSize);
    Task<ProductDto?> GetByIdAsync(int id);
    Task<(bool success, string? message, int? id)> CreateAsync(ProductCreateUpdateDto dto);
    Task<(bool success, string? message)> UpdateAsync(int id, ProductCreateUpdateDto dto);
    Task<(bool success, string? message)> DeleteAsync(int id);
}
