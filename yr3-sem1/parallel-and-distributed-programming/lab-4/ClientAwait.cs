using System.Net;
using System.Net.Sockets;
using System.Text;

namespace lab_4;

class ClientAwait
{
    // Download two files in parallel
    public static void Run()
    {
        var t1 = DownloadFileAsync("/file1.txt");
        var t2 = DownloadFileAsync("/file2.txt");
        Task.WaitAll(t1, t2); // Wait for both downloads to finish
        Console.ReadLine(); // Prevents the app from exiting immediately
    }

    static async Task DownloadFileAsync(string fileName)
    {
        var endPoint = new IPEndPoint(IPAddress.Loopback, 6767); // Connects on localhost:6767
        var conn = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        try
        {
            Console.WriteLine($"Attempting to connect for {fileName}. Started BeginConnect...");
            // await does 3 things:
            // 1. Checks if the awaited Task is already completed, if so, it continues executing the method synchronously
            // 2. If not completed, it pauses the method, returning a promise and control to the caller, allowing other
            // work to be done
            // 3. When the awaited Task completes, it resumes the method from where it was paused, restoring variables etc
            await ConnectAsync(conn, endPoint);
            Console.WriteLine($"Connected! ({fileName})");

            string request = $"GET {fileName} HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
            byte[] toSendBytes = Encoding.UTF8.GetBytes(request);
            await SendAsync(conn, toSendBytes);

            var response = new StringBuilder();
            var buffer = new byte[4096];
            int contentLength = -1;
            bool headersParsed = false;

            while (true)
            {
                int bytesRead = await ReceiveAsync(conn, buffer);
                if (bytesRead <= 0)
                    break;

                response.Append(Encoding.UTF8.GetString(buffer, 0, bytesRead));

                if (!headersParsed)
                {
                    string resp = response.ToString();
                    int headerEnd = resp.IndexOf("\r\n\r\n");
                    if (headerEnd != -1)
                    {
                        headersParsed = true;
                        string headers = resp[..headerEnd];
                        foreach (var line in headers.Split("\r\n"))
                        {
                            if (line.StartsWith("Content-Length:", System.StringComparison.OrdinalIgnoreCase))
                            {
                                var parts = line.Split(':');
                                if (parts.Length == 2 && int.TryParse(parts[1].Trim(), out int len))
                                    contentLength = len;
                            }
                        }
                    }
                }

                if (headersParsed && contentLength != -1)
                {
                    string resp = response.ToString();
                    int headerEnd = resp.IndexOf("\r\n\r\n");
                    int bodyStart = headerEnd + 4;
                    int bodyLength = resp.Length - bodyStart;
                    if (bodyLength >= contentLength)
                    {
                        string body = resp.Substring(bodyStart, contentLength);
                        Console.WriteLine($"File {fileName} content:\n{body}\n");
                        break;
                    }
                }
            }

            conn.Close();
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
            conn.Close();
        }
    }

    // IDE suggested we change it from Task to Task<bool> for improved performance
    // "Using concrete types leads to higher quality generated code by minimizing
    // virtual or interface dispatch overhead and enabling inlining."
    static Task<bool> ConnectAsync(Socket socket, EndPoint endPoint)
    {
        // TaskCompletionSource doesn't represent an ongoing operation by itself;
        // instead, it provides a way to manually create and complete a Task.
        // Here, it's used to wrap the BeginConnect/EndConnect callback pattern
        // into a Task-based abstraction, allowing us to signal completion or failure.
        var tcs = new TaskCompletionSource<bool>();
        socket.BeginConnect(endPoint, ar =>
        {
            try
            {
                socket.EndConnect(ar);
                tcs.SetResult(true);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }

    static Task<int> SendAsync(Socket socket, byte[] data)
    {
        var tcs = new TaskCompletionSource<int>();
        socket.BeginSend(data, 0, data.Length, SocketFlags.None, ar =>
        {
            try
            {
                int sent = socket.EndSend(ar);
                tcs.SetResult(sent);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }

    static Task<int> ReceiveAsync(Socket socket, byte[] buffer)
    {
        var tcs = new TaskCompletionSource<int>();
        socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
        {
            try
            {
                int received = socket.EndReceive(ar);
                tcs.SetResult(received);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        return tcs.Task;
    }
}