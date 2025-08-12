using System;
using System.IO;

namespace cs_a1a2
{
    // This class just reads from the provided file.
    internal class ReadFromFile
    {
        public string ReadFile(string filename, Action<string> processHeader, Action<string> processReadLine)
        {
            // We return the whole line as a string, the parsing is done separately. We
            // read the string in chunks as to avoid using a lot of memory
            using (StreamReader reader = new StreamReader(filename))
            {
                string readLine = reader.ReadLine();
                processHeader(readLine);
                readLine = reader.ReadLine();
                while (readLine != null)
                {
                    processReadLine(readLine);
                    readLine = reader.ReadLine();
                }
            }
            return "";
        }
    }
}
