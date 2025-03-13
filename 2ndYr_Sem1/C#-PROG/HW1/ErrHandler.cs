using System;

namespace cs_a1a2
{
    // ErrHandler serves to handle errors either from CL or File opening.
    internal class ErrHandler
    {
        public void HandleError(string errorType)
        {
            // Based on the type of the error, we will output a different error.
            WriteToSTDOUT writer = new WriteToSTDOUT();
            switch (errorType)
            {
                case "cl":
                    writer.WriteOutput("Argument Error");
                    Environment.Exit(0);
                    break;
                case "file":
                    writer.WriteOutput("File Error");
                    Environment.Exit(0);
                    break;
            }
        }
    }
}
