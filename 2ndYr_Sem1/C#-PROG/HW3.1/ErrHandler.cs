using System;

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

        public void HandleError(ERROR errorType)
        {
            // Based on the type of the error, we will output a different error.
            IOutputWriter writer = new OutputWriterToSTDOUT();
            switch (errorType)
            {
                case ERROR.CL:
                    writer.WriteOutput("Argument Error");
                    Environment.Exit(0);
                    break;
                case ERROR.FILE:
                    writer.WriteOutput("File Error");
                    Environment.Exit(0);
                    break;
                case ERROR.LINE:
                    writer.WriteOutput("Invalid File Format");
                    Environment.Exit(0);
                    break;
                case ERROR.HEADER:
                    writer.WriteOutput("Non-existent Column Name");
                    Environment.Exit(0);
                    break;
                case ERROR.VALUE:
                    writer.WriteOutput("Invalid Integer Value");
                    Environment.Exit(0);
                    break;
            }
        }
    }
}
