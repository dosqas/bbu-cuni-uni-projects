namespace LAB8.ViewModels;

public class CartItemViewModel
{
    public int ProductId { get; set; }
    public int Quantity { get; set; }
    public string Name { get; set; } = string.Empty;
    public decimal Price { get; set; }
}
