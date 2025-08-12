using LAB8.Data;
using LAB8.Models;
using LAB8.Models.DTOs;
using LAB8.Utils;
using LAB8.Services.Interfaces;
using Microsoft.EntityFrameworkCore;

namespace LAB8.Services;

public class ProductService(AppDbContext context) : IProductService
{
    private readonly AppDbContext _context = context;

    public async Task<(List<ProductDto> products, bool hasMore)> GetAllAsync(int? categoryId, int page, int pageSize)
    {
        var query = _context.Products.Include(p => p.Category).AsQueryable();

        if (categoryId.HasValue)
            query = query.Where(p => p.CategoryId == categoryId.Value);

        var total = await query.CountAsync();
        var products = await query
            .Skip((page - 1) * pageSize)
            .Take(pageSize)
            .Select(p => new ProductDto
            {
                Id = p.Id,
                Name = p.Name,
                Price = p.Price,
                Description = p.Description,
                CategoryName = p.Category!.Name
            })
            .ToListAsync();

        var hasMore = total > page * pageSize;
        return (products, hasMore);
    }

    public async Task<ProductDto?> GetByIdAsync(int id)
    {
        return await _context.Products
            .Include(p => p.Category)
            .Where(p => p.Id == id)
            .Select(p => new ProductDto
            {
                Id = p.Id,
                Name = p.Name,
                Price = p.Price,
                Description = p.Description,
                CategoryName = p.Category!.Name
            })
            .FirstOrDefaultAsync();
    }

    public async Task<(bool success, string? message, int? id)> CreateAsync(ProductCreateUpdateDto dto)
    {
        var validation = ProductUtils.ValidateProduct(dto);
        if (validation != null)
            return (false, (string)validation, null);

        var product = new Product
        {
            Name = dto.Name,
            Price = dto.Price,
            Description = dto.Description,
            CategoryId = dto.CategoryId
        };

        try
        {
            _context.Products.Add(product);
            await _context.SaveChangesAsync();
            return (true, "Product created successfully.", product.Id);
        }
        catch (Exception ex)
        {
            return (false, $"Database error: {ex.Message}", null);
        }
    }

    public async Task<(bool success, string? message)> UpdateAsync(int id, ProductCreateUpdateDto dto)
    {
        var product = await _context.Products.FindAsync(id);
        if (product == null)
            return (false, "Product not found.");

        var validation = ProductUtils.ValidateProduct(dto);
        if (validation != null)
            return (false, (string)validation);

        product.Name = dto.Name;
        product.Price = dto.Price;
        product.Description = dto.Description;
        product.CategoryId = dto.CategoryId;

        try
        {
            await _context.SaveChangesAsync();
            return (true, "Product updated successfully.");
        }
        catch (Exception ex)
        {
            return (false, $"Database error: {ex.Message}");
        }
    }

    public async Task<(bool success, string? message)> DeleteAsync(int id)
    {
        var product = await _context.Products.FindAsync(id);
        if (product == null)
            return (false, "Product not found.");

        try
        {
            _context.Products.Remove(product);
            await _context.SaveChangesAsync();
            return (true, "Product deleted successfully.");
        }
        catch (Exception ex)
        {
            return (false, $"Database error: {ex.Message}");
        }
    }
}
