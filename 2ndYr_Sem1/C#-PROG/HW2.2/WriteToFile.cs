using System;
using System.IO;
using System.Numerics;

namespace c_a1a2
{
    internal class WriteToFile
    {
        private string filename;
        StreamWriter writer;

        public WriteToFile(string filename)
        {
            this.filename = filename;
            this.writer = new StreamWriter(this.filename, true);
        }

        public void WriteOutput(string output)
        {
            this.writer.Write(output);
        }

        public void WriteOutput(int number)
        {
            this.writer.Write(number);
        }
    }
}

