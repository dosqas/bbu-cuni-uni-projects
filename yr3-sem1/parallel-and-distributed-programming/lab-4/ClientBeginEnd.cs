using System.Net;
using System.Net.Sockets;
using System.Text;

namespace lab_4;

class ClientBeginEnd
{
    class DownloadState
    {
        public Socket? Conn;
        public string FileName = "";
        public byte[] Buffer = new byte[4096];
        public StringBuilder Response = new();
        public bool HeadersParsed = false;
        public int ContentLength = -1;
    }

    // Download two files in parallel
    public static void Run()
    {
        DownloadFile("/file1.txt");
        DownloadFile("/file2.txt");
        Console.ReadLine(); // Prevents the app from exiting immediately
    }

    static void DownloadFile(string fileName)
    {
        var endPoint = new IPEndPoint(IPAddress.Loopback, 6767); // Connects on localhost:6767
        // Uses IPv4, TCP, and specifies the fact that the socket is a stream with a single peer
        var conn = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        try
        {
            Console.WriteLine($"Attempting to connect for {fileName}. Started BeginConnect...");
            var state = new DownloadState { Conn = conn, FileName = fileName };
            conn.BeginConnect(endPoint, ConnectCallback, state);
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex.Message);
        }
    }

    static void ConnectCallback(IAsyncResult ar)
    {
        // Once connected this gets called
        var state = (DownloadState)ar.AsyncState!;
        try
        {
            state.Conn!.EndConnect(ar);
            Console.WriteLine($"Connected! ({state.FileName})");

            string request = $"GET {state.FileName} HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
            byte[] toSendBytes = Encoding.UTF8.GetBytes(request);
            // Sends toSendBytes from offset 0 to toSendBytes.Length with no special flags
            state.Conn.BeginSend(toSendBytes, 0, toSendBytes.Length, SocketFlags.None, SendCallback, state);
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
        }
    }

    static void SendCallback(IAsyncResult ar)
    {
        // Once we send the data to the server this gets called
        var state = (DownloadState)ar.AsyncState!;
        try
        {
            int sent = state.Conn!.EndSend(ar);
            Console.WriteLine($"Sent {sent} bytes. ({state.FileName})");

            // The last parameter, state, is the state object (DownloadState) that will be passed to ReceiveCallback
            state.Conn.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
        }
    }

    static void ReceiveCallback(IAsyncResult ar)
    {
        // Once we receive data from the server this gets called
        var state = (DownloadState)ar.AsyncState!;
        try
        {
            // We unpack the bytes of the passed objects by using the AsyncState property
            int retBytes = state.Conn!.EndReceive(ar);
            if (retBytes > 0)
            {
                state.Response.Append(Encoding.UTF8.GetString(state.Buffer, 0, retBytes));

                if (!state.HeadersParsed)
                {
                    string resp = state.Response.ToString();
                    int headerEnd = resp.IndexOf("\r\n\r\n");
                    if (headerEnd != -1)
                    {
                        state.HeadersParsed = true;
                        string headers = resp[..headerEnd];
                        foreach (var line in headers.Split("\r\n"))
                        {
                            if (line.StartsWith("Content-Length:", System.StringComparison.OrdinalIgnoreCase))
                            {
                                var parts = line.Split(':');
                                if (parts.Length == 2 && int.TryParse(parts[1].Trim(), out int len))
                                    state.ContentLength = len;
                            }
                        }
                    }
                }

                if (state.HeadersParsed && state.ContentLength != -1)
                {
                    string resp = state.Response.ToString();
                    int headerEnd = resp.IndexOf("\r\n\r\n");
                    int bodyStart = headerEnd + 4; // Skip past the \r\n\r\n
                    int bodyLength = resp.Length - bodyStart;
                    if (bodyLength >= state.ContentLength)
                    {
                        string body = resp.Substring(bodyStart, state.ContentLength);
                        Console.WriteLine($"File {state.FileName} content:\n{body}\n");
                        state.Conn.Close();
                        return;
                    }
                }

                state.Conn.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
            }
            else
            {
                state.Conn.Close();
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
        }
    }
}