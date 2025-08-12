using System.Text;
using System;
using System.IO;

#nullable enable

namespace cs_assignment3
{
    // This class serves to only print to the standard output.
    public class OutputWriterToSTDOUT : IOutputWriter
    {

        public void WriteOutput(string output)
        {
            // Just prints to standard output a string.
            Console.WriteLine(output);
        }

        public void WriteOutput(int number)
        {
            // We need to treat int outputs differently.
            Console.WriteLine(number.ToString());
        }
    }
}
