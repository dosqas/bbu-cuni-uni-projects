using System.Net;
using System.Net.Sockets;
using System.Text;

namespace lab_4;

class ClientTaskLoop
{
    public static void Run()
    {
        var t1 = DownloadFileTask("/file1.txt");
        var t2 = DownloadFileTask("/file2.txt");
        Task.WaitAll(t1, t2); // Wait for both downloads to finish
        Console.ReadLine();
    }

    static Task<bool> DownloadFileTask(string fileName)
    {
        var endPoint = new IPEndPoint(IPAddress.Loopback, 6767);
        var conn = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        Console.WriteLine($"Attempting to connect for {fileName}. Started BeginConnect...");
        var response = new StringBuilder();
        var buffer = new byte[4096];
        int contentLength = -1;
        bool headersParsed = false;

        // Chain the tasks using ContinueWith
        // The ContinueWith method is used to chain tasks together, where each task starts after the previous one completes.
        // Compared to async/await, ContinueWith can lead to more complex and less readable code, especially with multiple
        // continuations.
        // Moreover, await is compiler optimized for handling asynchronous code, since it generates state machines to manage
        // the asynchronous flow, avoid creating unnecessary lambdas/delegates, inlines code better, and provides clearer
        // stack traces for exceptions since it repropagates exceptions directly to the awaiter.
        // ContinueWith also schedules on the thread pool by default, even if in a GUI or ASP.NET context (not the case here but
        // worth mentioning)
        // Another interesting thing is that ContinueWith does not capture the current synchronization context by default,
        // which can make it avoid some deadlock scenarios that can occur with await in certain contexts, but it can also
        // lead to running continuations on unexpected threads (since it joins the thread pool).
        // Since await always resumes on the captured context (which can be the UI thread or ASP.NET request context),
        // it can lead to deadlocks if not handled carefully.

        // Unwrap is used to flatten nested tasks (Task<Task<T>> to Task<T>)
        // For example, we return a Task<bool> from the ConnectAsync method, and then in the continuation,
        // we return another Task from SendAsync. Without Unwrap, we would end up with a Task<Task<bool>>,
        return ConnectAsync(conn, endPoint)
            .ContinueWith(connectTask =>
            {
                Console.WriteLine($"Connected! ({fileName})");
                string request = $"GET {fileName} HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
                byte[] toSendBytes = Encoding.UTF8.GetBytes(request);
                return SendAsync(conn, toSendBytes);
            })
            .Unwrap()
            .ContinueWith(sendTask =>
            {
                // Loop for receiving data
                // We need this for this approach and not for the async/await one since we don't have
                // the luxury of using a simple while(true) loop with await inside
                TaskCompletionSource<bool> done = new();
                void ReceiveLoop()
                {
                    ReceiveAsync(conn, buffer).ContinueWith(receiveTask =>
                    {
                        int bytesRead = receiveTask.Result;
                        if (bytesRead <= 0)
                        {
                            conn.Close();
                            done.SetResult(true);
                            return;
                        }

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
                                conn.Close();
                                done.SetResult(true);
                                return;
                            }
                        }

                        ReceiveLoop();
                    });
                }
                ReceiveLoop();
                return done.Task;
            })
            .Unwrap();
    }

    static Task<bool> ConnectAsync(Socket socket, EndPoint endPoint)
    {
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