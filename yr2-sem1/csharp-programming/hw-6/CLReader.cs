namespace cs_assignment3
{
    // Simple class used to read the commandline arguments.
    public class CLReader
    {
        private string[] _args { get; init; }
        private int _index;

        public CLReader(string[] args)
        {
            _args = args;
            _index = 0;
        }

        // Using the out keyword, we can update the values of the variables passed to this method, outside of it.
        public void ReadArgs(out string inputFile, out string outputFile, out int columnName) 
        {
            inputFile  = _args[0]; 
            outputFile = _args[1];
            columnName = int.Parse(_args[2]);
        }
        public void ReadInitialArgsMultiFile(out bool enableHighlight, out string outputFile, out int columnName)
        {
            if (_args[_index] == "--highlight-spaces")
            {
                _index++;
                enableHighlight = true;
            }
            else enableHighlight = false;
            outputFile = _args[_args.Length - 2];
            columnName = int.Parse(_args[_args.Length - 1]);
        }

        public bool ReadNextInputFileArgMultiFile(out string nextInputFile)
        {
            if (_index == _args.Length - 2)
            {
                nextInputFile = "";
                return false;
            }
            nextInputFile = _args[_index++];
            return true;
        }
    }
}
