using LAB8.Models.DTOs;
using LAB8.Services.Interfaces;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

namespace LAB8.Controllers;

[ApiController]
[Authorize]
[Route("api/products")]
public class ProductController(IProductService productService) : ControllerBase
{
    private const int PageSize = 4;

    [HttpGet]
    public async Task<IActionResult> GetAll([FromQuery] int? category, [FromQuery] int page = 1)
    {
        var (products, hasMore) = await productService.GetAllAsync(category, page, PageSize);
        return Ok(new { success = true, products, hasMore });
    }

    [HttpGet("{id:int}")]
    public async Task<IActionResult> GetById(int id)
    {
        var product = await productService.GetByIdAsync(id);
        if (product == null)
            return NotFound(new { success = false, message = "Product not found." });

        return Ok(new { success = true, product });
    }

    [HttpPost]
    public async Task<IActionResult> Create([FromBody] ProductCreateUpdateDto dto)
    {
        var (success, message, id) = await productService.CreateAsync(dto);
        if (!success)
            return BadRequest(new { success = false, message });

        return CreatedAtAction(nameof(GetById), new { id }, new { success = true, message, id });
    }

    [HttpPut("{id:int}")]
    public async Task<IActionResult> Update(int id, [FromBody] ProductCreateUpdateDto dto)
    {
        var (success, message) = await productService.UpdateAsync(id, dto);
        if (!success)
            return BadRequest(new { success = false, message });

        return Ok(new { success = true, message });
    }

    [HttpDelete("{id:int}")]
    public async Task<IActionResult> Delete(int id)
    {
        var (success, message) = await productService.DeleteAsync(id);
        if (!success)
            return NotFound(new { success = false, message });

        return Ok(new { success = true, message });
    }
}
