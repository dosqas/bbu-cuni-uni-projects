namespace lab_4;

class Program
{
    static void Main(string[] args)
    {
        if (args.Length > 0 && args[0] == "server")
        {
            Server.Run();
        }
        else if (args.Length > 0 && args[0] == "client1")
        {
            ClientBeginEnd.Run();
        }
        else if (args.Length > 0 && args[0] == "client2")
        {
            ClientTaskLoop.Run();
        }
        else if (args.Length > 0 && args[0] == "client3")
        {
            ClientAwait.Run();
        }
        else
        {
            Console.WriteLine("Usage: <server|client1|client2|client3>");
        }
    }
}