using System;
using System.IO;

namespace cs_assignment3
{
    // ErrHandler serves to handle errors either from input or from the data format.
    public class ErrHandler
    {
        // Enum to represent the different types of errors that can occur.
        // CL: Command Line Argument Error
        // FILE: File Error
        // LINE: Invalid File Format
        // HEADER: Non-existent Column Name
        // VALUE: Value cannot be represented as an int32
        public enum ERROR { CL, FILE, LINE, HEADER, VALUE }

        public Exception HandleError(ERROR errorType)
        {
            switch (errorType)
            {
                case ERROR.CL:
                    return new ArgumentException("Argument Error");
                case ERROR.FILE:
                    return new IOException("File Error");
                case ERROR.LINE:
                    return new FormatException("Invalid File Format");
                case ERROR.HEADER:
                    return new InvalidOperationException("Non-existent Column Name");
                case ERROR.VALUE:
                    return new FormatException("Invalid Integer Value");
                default:
                    return new Exception("WTF?");
            }
        }

        public void HandleException(Exception e)
        {
            if (e.Message == "WTF?")
                Environment.Exit(-100);
            Console.WriteLine(e.Message);
            Environment.Exit(0);
        }
    }
}
