using LAB8.Models.DTOs;

namespace LAB8.Utils;

public static class ProductUtils
{
    public static object? ValidateProduct(ProductCreateUpdateDto product)
    {
        if (string.IsNullOrWhiteSpace(product.Name) || product.Name.Length > 40)
            return new { success = false, message = "Product name must be between 1 and 40 characters." };

        if (product.Description?.Length > 200)
            return new { success = false, message = "Description must not exceed 200 characters." };

        if (product.CategoryId <= 0)
            return new { success = false, message = "Invalid category selected." };

        if (product.Price < 0)
            return new { success = false, message = "Invalid price." };

        return null;
    }
}