using System;
using System.IO;

#nullable enable

namespace cs_assignment3
{
    // This class serves to write to a file.
    public class OutputWriterToFile : IOutputWriter, IDisposable
    {
        private StreamWriter _writer { get; init; }

        // Constructor. We need to pass the filename to write to.
        public OutputWriterToFile(string filename)
        {
            _writer = new StreamWriter(filename);
        }

        // We write the output, which is a string, to the file.
        public void WriteOutput(string output)
        {
            _writer.WriteLine(output);
            // Ensure the data is written to the file immediately.
            _writer.Flush();
        }

        // Dealing with ints in a different way as opposed to strings.
        public void WriteOutput(int number)
        {
            _writer.WriteLine(number);
            // Ensure the data is written to the file immediately.
            _writer.Flush();
        }

        // We implement IDisposable to ensure the StreamWriter is properly disposed of.
        // (since we do not use the 'using' keyword with it)
        public void Dispose()
        {
            _writer?.Dispose();
        }
    }
}

