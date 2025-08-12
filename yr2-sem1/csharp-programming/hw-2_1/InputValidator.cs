using System;
using System.IO;

namespace cs_a1a2
{
    // InputValidator acts to validate our inputs: CommandLine and File.
    internal class InputValidator
    {
        public bool ValidateCommandLineArguments(string[] args)
        {
            // We are expecting only one argument from the commandline, no more no less ;).
            return args.Length == 1;
        }

        public bool ValidateFile(string filename)
        {
            try
            {
                // Checking if we can open the file for reading. If we can do it, return true. Otherwise, return false.
                using (FileStream filestream = new FileStream(filename, FileMode.Open, FileAccess.Read))
                {
                    return true;
                }
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}
