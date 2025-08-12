using System;
using System.IO;

namespace cs_assignment3
{
    // InputValidator acts to validate input data.
    public class ValidatorInput
    {
        public bool ValidateCommandLineArguments(string[] args)
        {
            // We are expecting only a certain number of arguments.
            return args.Length == 1;
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
    }
}
