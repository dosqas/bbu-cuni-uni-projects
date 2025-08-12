using System;
using System.Collections.Generic;
using System.IO;

#nullable enable

namespace NezarkaBookstore
{
    class ClientRequestReader
    {
        public void ReadRequests(ModelStore store)
        {            
            Customer customer;
            Book book;
            Request.RequestType requestType;
            String? line;

            while ((line = Console.ReadLine()) != null)
            {

                String[] tokens = line.Split(' ', 3);
                if (!ParseAndValidateRequest(tokens, store, out customer, out book, out requestType))
                    requestType = 0;

                ClientRequestHandler handler = new ClientRequestHandler();

                handler.HandleRequest(new Request((Request.RequestType)requestType, customer, book), store);
            }

            Environment.Exit(0);
        }

        private bool ParseAndValidateRequest(string[] tokens, ModelStore store, out Customer? customer, out Book? book, out Request.RequestType requestType)
        {
            customer = null;
            book = null;
            requestType = Request.RequestType.InvalidRequest;

            if (tokens.Length != 3) return false;
            if (tokens[0] != "GET") return false;

            try
            {
                int customerId = int.Parse(tokens[1]);
                customer = store.GetCustomer(customerId);
                if (customer == null) return false;
            }
            catch (FormatException)
            {
                return false;
            }
            catch (ArgumentNullException)
            {
                return false;
            }

            string path = tokens[2].Replace("http://www.nezarka.net/", "");

            if (path == "Books")
            {
                requestType = Request.RequestType.ShowAllBooks;
                return true;
            }
            if (path == "ShoppingCart")
            {
                requestType = Request.RequestType.ShowShoppingCart;
                return true;
            }

            if (path.StartsWith("Books/Detail/"))
            {
                requestType = Request.RequestType.BookDetails;
                path = path.Replace("Books/Detail/", "");
            }
            else if (path.StartsWith("ShoppingCart/Add/"))
            {
                requestType = Request.RequestType.AddToShoppingCart;
                path = path.Replace("ShoppingCart/Add/", "");
            }
            else if (path.StartsWith("ShoppingCart/Remove/"))
            {
                requestType = Request.RequestType.RemoveFromShoppingCart;
                path = path.Replace("ShoppingCart/Remove/", "");
            }
            else
            {
                return false;
            }

            try
            {
                int bookId = int.Parse(path);
                book = store.GetBook(bookId);
                if (book == null) return false;
            }
            catch (FormatException)
            {
                return false;
            }
            catch (ArgumentNullException)
            {
                return false;
            }

            return true;
        }

    }
}
