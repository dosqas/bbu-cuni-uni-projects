using System;
using System.Collections.Generic; 
using System.IO;
using System.Linq;


namespace NezarkaBookstore
{
    class ClientRequestHandler
    {
        public void HandleRequest(Request request, ModelStore store)
        {
            ResultWriter resultWriter = new ResultWriter();

            if (request.Type == Request.RequestType.InvalidRequest || request.Customer == null)
            {
                resultWriter.PrintInvalidRequest();
                return;
            }

            String customerFirstName = request.Customer.FirstName;
            int customerCartCount = request.Customer.ShoppingCart.Items.Count;

            switch (request.Type)
            {
                case Request.RequestType.ShowAllBooks:
                    {
                        List<Book> books = (List<Book>)store.GetBooks();

                        resultWriter.PrintAllBooks(customerFirstName, customerCartCount, books);
                        return;
                    }
                case Request.RequestType.BookDetails:
                    {
                        if (request.Book == null)
                        {
                            resultWriter.PrintInvalidRequest();
                            return;
                        }
                        Book book = store.GetBook(request.Book.Id);

                        resultWriter.PrintBookDetails(customerFirstName, customerCartCount, book);
                        return;
                    }
                case Request.RequestType.ShowShoppingCart:
                    {
                        decimal finalPrice;
                        List<ShoppingCartItem> shoppingCartItems = request.Customer.ShoppingCart.Items;

                        var cartItemsWithBooks = GetCartItemsWithBooks(shoppingCartItems, store, out finalPrice);

                        var books = cartItemsWithBooks.Select(pair => pair.Book).ToList();
                        var items = cartItemsWithBooks.Select(pair => pair.Item).ToList();

                        resultWriter.PrintShoppingCartDetails(customerFirstName, customerCartCount, items, books, finalPrice);
                        return;
                    }
                case Request.RequestType.AddToShoppingCart:
                    {
                        if (request.Book == null)
                        {
                            resultWriter.PrintInvalidRequest();
                            return;
                        }

                        List<ShoppingCartItem> shoppingCartItems = request.Customer.ShoppingCart.Items;
                        AddToCart(shoppingCartItems, request.Book.Id);

                        decimal finalPrice;
                        var cartItemsWithBooks = GetCartItemsWithBooks(shoppingCartItems, store, out finalPrice);
                        customerCartCount = shoppingCartItems.Count;

                        var books = cartItemsWithBooks.Select(pair => pair.Book).ToList();
                        var items = cartItemsWithBooks.Select(pair => pair.Item).ToList();

                        resultWriter.PrintShoppingCartDetails(customerFirstName, customerCartCount, items, books, finalPrice);
                        return;
                    }
                case Request.RequestType.RemoveFromShoppingCart:
                    {
                        if (request.Book == null)
                        {
                            resultWriter.PrintInvalidRequest();
                            return;
                        }

                        List<ShoppingCartItem> shoppingCartItems = request.Customer.ShoppingCart.Items;
                        bool result = RemoveFromCart(shoppingCartItems, request.Book.Id);

                        decimal finalPrice;
                        var cartItemsWithBooks = GetCartItemsWithBooks(shoppingCartItems, store, out finalPrice);
                        customerCartCount = shoppingCartItems.Count;

                        if (result)
                        {
                            var books = cartItemsWithBooks.Select(pair => pair.Book).ToList();
                            var items = cartItemsWithBooks.Select(pair => pair.Item).ToList();

                            resultWriter.PrintShoppingCartDetails(customerFirstName, customerCartCount, items, books, finalPrice);
                        }
                        else
                        {
                            resultWriter.PrintInvalidRequest();
                        }

                        return;
                    }

            }
        }

        private List<(ShoppingCartItem Item, Book Book)> GetCartItemsWithBooks(List<ShoppingCartItem> shoppingCartItems, ModelStore store, out decimal finalPrice)
        {
            finalPrice = 0;
            var cartItemsWithBooks = new List<(ShoppingCartItem, Book)>();

            foreach (var item in shoppingCartItems)
            {
                Book book = store.GetBook(item.BookId);

                if (book != null)
                {
                    cartItemsWithBooks.Add((item, book));
                    finalPrice += book.Price * item.Count;
                }
            }

            return cartItemsWithBooks;
        }


        private void AddToCart(List<ShoppingCartItem> items, int bookId)
        {
            foreach (var item in items)
            {
                if (item.BookId == bookId)
                {
                    item.Count++;
                    return;
                }
            }

            ShoppingCartItem newItem = new ShoppingCartItem
            {
                BookId = bookId,
                Count = 1
            };
            items.Add(newItem);
        }
        private bool RemoveFromCart(List<ShoppingCartItem> items, int bookId)
        {
            foreach (var item in items)
            {
                if (item.BookId == bookId)
                {
                    if (item.Count == 1)
                    {
                        items.Remove(item);
                    }
                    else
                    {
                        item.Count--;
                    }

                    return true;
                }
            }

            return false;
        }

    }
}
