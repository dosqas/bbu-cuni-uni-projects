using System;
using System.Collections.Generic; 
using System.IO;

#nullable enable

namespace NezarkaBookstore
{
    public class Request
    {
        public enum RequestType
        {
            InvalidRequest,
            ShowAllBooks,
            BookDetails,
            ShowShoppingCart,
            AddToShoppingCart,
            RemoveFromShoppingCart
        }

        public RequestType Type { get; set; }
        public Customer? Customer { get; set; }
        public Book? Book { get; set; }

        public Request(RequestType requestType, Customer? customer, Book? book)
        {
            Type = requestType;

            switch (requestType)
            {
                case RequestType.InvalidRequest:
                    Customer = null;
                    Book = null;
                    break;

                case RequestType.ShowAllBooks:
                    Customer = customer;
                    Book = null;
                    break;

                case RequestType.BookDetails:
                    Customer = customer;
                    Book = book;
                    break;

                case RequestType.ShowShoppingCart:
                    Customer = customer;
                    Book = null;
                    break;

                case RequestType.AddToShoppingCart:
                    Customer = customer;
                    Book = book;
                    break;

                case RequestType.RemoveFromShoppingCart:
                    Customer = customer;
                    Book = book;
                    break;
            }
        }
    }
}
