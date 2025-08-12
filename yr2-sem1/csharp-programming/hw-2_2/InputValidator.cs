using System;
using System.IO;

namespace cs_a1a2
{
    // InputValidator acts to validate our inputs: CommandLine, File, Line, Header, and Value.
    internal class InputValidator
    {
        public bool ValidateCommandLineArguments(string[] args, int nrOfArgs)
        {
            // We are expecting only nrOfArgs arguments.
            return args.Length == nrOfArgs;
        }

        public bool ValidateFile(string filename)
        {
            try
            {
                // Checking if we can open the file for reading. If we can do it, return true. Otherwise, return false.
                using (FileStream filestream = new FileStream(filename, FileMode.Open, FileAccess.ReadWrite))
                {
                    return true;
                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        public bool ValidateLine(int lineWords, int nrOfColumns)
        {
            // We are checking if the line got the same number of columns as the header.
            return lineWords == nrOfColumns;
        }

        public bool ValidateHeader(string[] header, string column)
        {
            // We are checking if the column exists in the header.
            foreach (string word in header)
            {
                if (word == column)
                {
                    return true;
                }
            }
            return false;
        }

        public bool ValidateValue(string value)
        {
            // We are checking if the value can be represented as a 32-bit signed integer.
            return int.TryParse(value, out _);
        }
    }
}
