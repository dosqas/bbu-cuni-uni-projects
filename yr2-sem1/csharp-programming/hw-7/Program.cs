using System;
using System.Collections.Generic;
using System.IO;

namespace NezarkaBookstore
{
    internal class Program
    {
        static void Main(string[] args)
        {
            ModelStore store = new ModelStore();
            store = ModelStore.LoadFrom(Console.In);

            if (store == null) { Console.WriteLine("Data error."); return; }
            else
            {
                ClientRequestReader reader = new ClientRequestReader();

                reader.ReadRequests(store);
            }
        }
    }
}
