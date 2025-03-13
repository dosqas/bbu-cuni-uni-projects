using System;
using System.IO;

namespace cs_assignment3
{
    // InputValidator acts to validate input data.
    public class ValidatorInput
    {
        private ErrHandler _errHandler;

        public ValidatorInput()
        {
            _errHandler = new ErrHandler();
        }

        public bool ValidateCommandLineArguments(string[] args)
        {
            // We are expecting only a certain number of arguments.
            if (args.Length == 3)
                return true;
            else throw _errHandler.HandleError(ErrHandler.ERROR.CL);
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
                throw _errHandler.HandleError(ErrHandler.ERROR.FILE);
            }
        }
    }
}
