namespace cs_assignment3
{
    // This interface is used to define the methods that will be used to write output.
    // Output is written to either a file or the standard output.
    // We only deal with printing strings and integers.
    public interface IOutputWriter
    {
        void WriteOutput(string output);
        void WriteOutput(int number);
    }
}
