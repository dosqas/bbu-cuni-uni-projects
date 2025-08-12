using c_a1a2;

namespace cs_a1a2
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            InputValidator valid = new InputValidator();
            ErrHandler errHandle = new ErrHandler();

            bool validity = valid.ValidateCommandLineArguments(args);
            if (!validity)
            {
                errHandle.HandleError("cl");
            }

            ReadFromCL readCl = new ReadFromCL();
            string filename = readCl.ReadArgs(args);

            validity = valid.ValidateFile(filename);
            if (!validity)
            {
                errHandle.HandleError("file");
            }

            // ASSIGNMENT 1
            /*ReadFromFile readFile = new ReadFromFile();
            SeparateStringInWords separator = new SeparateStringInWords();
            CalculateWC calculator = new CalculateWC();

            readFile.ReadFile(filename, readLine =>
            {
                string[] separatedString = separator.SeparatingString(readLine);
                calculator.AddCount(separatedString.Length);
            });

            int result = calculator.Calculate();

            WriteToSTDOUT writer = new WriteToSTDOUT();
            writer.WriteOutput(result);*/

            // ASSIGNMENT 2
            ReadFromFile readFile = new ReadFromFile();
            SeparateStringInWords separator = new SeparateStringInWords();
            WordFreqDictionary frequencies = new WordFreqDictionary();

            readFile.ReadFile(filename, readLine =>
            {
                string[] separatedString = separator.SeparatingString(readLine);
                frequencies.addWords(separatedString);
            });

            WriteToSTDOUT writer = new WriteToSTDOUT();
            writer.WriteOutput(frequencies.getWords());
        }
    }
}
