namespace cs_a1a2
{
    // We use this class to read the commandline. It is very simple.
    internal class ReadFromCL
    {
        public string ReadArgs(string[] args)
        {
            // Making a class for just reading the commandline arguments lets us easily modify it if we need to.
            // If we need to do more reading of arguments or more complex operations, we could easily do it now,
            // an advantage to reading them in main.
            return args[0];
        }
    }
}
