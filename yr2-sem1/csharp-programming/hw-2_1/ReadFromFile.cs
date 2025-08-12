using System;
using System.IO;

namespace cs_a1a2
{
    // This class just reads from the provided file.
    internal class ReadFromFile
    {
        public void ReadFile(string filename, Action<char> processReadChar)
        {
            // We read char by char, sending over to processReadChar to form it in a word.
            using (StreamReader reader = new StreamReader(filename))
            {
                int readChar;
                while ((readChar = reader.Read()) != -1)
                {
                    char c = (char)readChar;
                    processReadChar(c);
                }
            }
        }
    }
}
