using System.Net;
using System.Net.Sockets;
using System.Text;

namespace lab_4;

class Server
{
    public static void Run()
    {
        var listener = new TcpListener(IPAddress.Loopback, 6767);
        listener.Start();
        Console.WriteLine("Server started on localhost:6767");

        while (true)
        {
            var client = listener.AcceptTcpClient();
            Task.Run(() => HandleClient(client));
        }
    }

    static void HandleClient(TcpClient client)
    {
        using var stream = client.GetStream();
        var buffer = new byte[4096];
        int bytesRead = stream.Read(buffer, 0, buffer.Length);
        string request = Encoding.UTF8.GetString(buffer, 0, bytesRead);

        // Parse the GET line
        string[] lines = request.Split("\r\n");
        string getLine = lines[0];
        string[] parts = getLine.Split(' ');
        string fileName = parts.Length > 1 ? parts[1].TrimStart('/') : "";

        if (string.IsNullOrWhiteSpace(fileName) || !File.Exists(fileName))
        {
            // File not found
            string notFound = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
            byte[] notFoundBytes = Encoding.UTF8.GetBytes(notFound);
            stream.Write(notFoundBytes, 0, notFoundBytes.Length);
        }
        else
        {
            // File found
            byte[] fileBytes = File.ReadAllBytes(fileName);
            string header = $"HTTP/1.1 200 OK\r\nContent-Length: {fileBytes.Length}\r\nConnection: close\r\n\r\n";
            byte[] headerBytes = Encoding.UTF8.GetBytes(header);
            stream.Write(headerBytes, 0, headerBytes.Length);
            stream.Write(fileBytes, 0, fileBytes.Length);
        }

        client.Close();
    }
}