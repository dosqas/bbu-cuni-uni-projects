using System;
using System.Collections.Generic;
using System.IO;

namespace NezarkaBookstore
{
    class ResultWriter
    {
        public void PrintInvalidRequest()
        {
            Console.WriteLine("<!DOCTYPE html>");
            Console.WriteLine("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">");
            Console.WriteLine("<head>");
            Console.WriteLine("\t<meta charset=\"utf-8\" />");
            Console.WriteLine("\t<title>Nezarka.net: Online Shopping for Books</title>");
            Console.WriteLine("</head>");
            Console.WriteLine("<body>");
            Console.WriteLine("<p>Invalid request.</p>");
            Console.WriteLine("</body>");
            Console.WriteLine("</html>");

            Console.WriteLine("====");
        }

        public void PrintCommonHeader(String name, int cartCount)
        {
            Console.WriteLine("<!DOCTYPE html>");
            Console.WriteLine("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">");
            Console.WriteLine("<head>");
            Console.WriteLine("\t<meta charset=\"utf-8\" />");
            Console.WriteLine("\t<title>Nezarka.net: Online Shopping for Books</title>");
            Console.WriteLine("</head>");
            Console.WriteLine("<body>");
            Console.WriteLine("\t<style type=\"text/css\">");
            Console.WriteLine("\t\ttable, th, td {");
            Console.WriteLine("\t\t\tborder: 1px solid black;");
            Console.WriteLine("\t\t\tborder-collapse: collapse;");
            Console.WriteLine("\t\t}");
            Console.WriteLine("\t\ttable {");
            Console.WriteLine("\t\t\tmargin-bottom: 10px;");
            Console.WriteLine("\t\t}");
            Console.WriteLine("\t\tpre {");
            Console.WriteLine("\t\t\tline-height: 70%;");
            Console.WriteLine("\t\t}");
            Console.WriteLine("\t</style>");
            Console.WriteLine("\t<h1><pre>  v,<br />Nezarka.NET: Online Shopping for Books</pre></h1>");
            Console.WriteLine($"\t{name}, here is your menu:");
            Console.WriteLine("\t<table>");
            Console.WriteLine("\t\t<tr>");
            Console.WriteLine("\t\t\t<td><a href=\"/Books\">Books</a></td>");
            Console.WriteLine($"\t\t\t<td><a href=\"/ShoppingCart\">Cart ({cartCount})</a></td>");
            Console.WriteLine("\t\t</tr>");
            Console.WriteLine("\t</table>");

        }

        public void PrintAllBooks(String customerFirstName, int customerCartCount, List<Book> books)
        {
            PrintCommonHeader(customerFirstName, customerCartCount);
            Console.WriteLine("\tOur books for you:");
            Console.WriteLine("\t<table>");

            if (books.Count > 0)
            {
                int index = 0;
                Console.WriteLine("\t\t<tr>");
                while (index < books.Count - 1)
                {
                    FormatAndPrintBook(index, books[index]);
                    if (index % 3 == 2)
                    {
                        Console.WriteLine("\t\t</tr>");
                        Console.WriteLine("\t\t<tr>");
                    }
                    index++;
                }
                FormatAndPrintBook(index, books[index]);
                Console.WriteLine("\t\t</tr>");
            }


            Console.WriteLine("\t</table>");
            Console.WriteLine("</body>");
            Console.WriteLine("</html>");


            Console.WriteLine("====");
        }

        private void FormatAndPrintBook(int index, Book book) 
        {
            Console.WriteLine("\t\t\t<td style=\"padding: 10px;\">");
            Console.WriteLine($"\t\t\t\t<a href=\"/Books/Detail/{book.Id}\">{book.Title}</a><br />");
            Console.WriteLine($"\t\t\t\tAuthor: {book.Author}<br />");
            Console.WriteLine($"\t\t\t\tPrice: {book.Price} EUR &lt;<a href=\"/ShoppingCart/Add/{book.Id}\">Buy</a>&gt;");
            Console.WriteLine("\t\t\t</td>");
        }

        public void PrintBookDetails(String clientFirstName, int customerCartCount, Book book)
        {
            PrintCommonHeader(clientFirstName, customerCartCount);

            Console.WriteLine("\tBook details:");
            Console.WriteLine($"\t<h2>{book.Title}</h2>");
            Console.WriteLine("\t<p style=\"margin-left: 20px\">");
            Console.WriteLine($"\tAuthor: {book.Author}<br />");
            Console.WriteLine($"\tPrice: {book.Price} EUR<br />");
            Console.WriteLine("\t</p>");
            Console.WriteLine($"\t<h3>&lt;<a href=\"/ShoppingCart/Add/{book.Id}\">Buy this book</a>&gt;</h3>");
            Console.WriteLine("</body>");
            Console.WriteLine("</html>");

            Console.WriteLine("====");
        }

        public void PrintShoppingCartDetails(String clientFirstName, int customerCartCount, List<ShoppingCartItem> items, List<Book> books, decimal totalPrice)
        {
            PrintCommonHeader(clientFirstName, customerCartCount);

            if (items.Count > 0)
            {
                Console.WriteLine("\tYour shopping cart:");
                Console.WriteLine("\t<table>");
                Console.WriteLine("\t\t<tr>");
                Console.WriteLine("\t\t\t<th>Title</th>");
                Console.WriteLine("\t\t\t<th>Count</th>");
                Console.WriteLine("\t\t\t<th>Price</th>");
                Console.WriteLine("\t\t\t<th>Actions</th>");
                Console.WriteLine("\t\t</tr>");

                for (int i = 0; i < items.Count; i++)
                {
                    FormatAndPrintItemInCart(books[i], items[i].Count);
                }

                Console.WriteLine("\t</table>");
                Console.WriteLine($"\tTotal price of all items: {totalPrice} EUR");

            }
            else Console.WriteLine("\tYour shopping cart is EMPTY.");

            Console.WriteLine("</body>");
            Console.WriteLine("</html>");

            Console.WriteLine("====");
        }

        private void FormatAndPrintItemInCart(Book book, int count)
        {
            Console.WriteLine("\t\t<tr>");
            Console.WriteLine($"\t\t\t<td><a href=\"/Books/Detail/{book.Id}\">{book.Title}</a></td>");
            Console.WriteLine($"\t\t\t<td>{count}</td>");

            if (count == 1) Console.WriteLine($"\t\t\t<td>{book.Price} EUR</td>");
            else Console.WriteLine($"\t\t\t<td>{count} * {book.Price} = {count * book.Price} EUR</td>");

            Console.WriteLine($"\t\t\t<td>&lt;<a href=\"/ShoppingCart/Remove/{book.Id}\">Remove</a>&gt;</td>");
            Console.WriteLine("\t\t</tr>");
        }
    }
}
