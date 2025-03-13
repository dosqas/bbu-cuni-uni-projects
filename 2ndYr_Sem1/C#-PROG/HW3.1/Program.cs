using System;
using System.IO;
using cs_assignment3;

namespace cs_assignment3
{
    public class Program
    {
        public static void Main(string[] args)
        {
            ValidatorInput inputValidator = new ValidatorInput();
            ErrHandler errHandler = new ErrHandler();

            if (!inputValidator.ValidateCommandLineArguments(args))
            {
                errHandler.HandleError(ErrHandler.ERROR.CL);
            }

            string inputFile;

            CLReader clReader = new CLReader(args);
            clReader.ReadArgs(out inputFile);

            if (!inputValidator.ValidateFile(inputFile))
            {
                errHandler.HandleError(ErrHandler.ERROR.FILE);
            }

            char[] delimiters = { ' ', '\n', '\t', '\r' };
            StreamReader inputFileReader = new StreamReader(inputFile);
            ITokenReader tokenReader = new TokenReaderByChar(inputFileReader, delimiters);
            ITokenProcessor paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            paragraphWordCounter.ProcessAllTokens();

            IOutputWriter writeToFile = new OutputWriterToSTDOUT();
            paragraphWordCounter.WriteReport(writeToFile);

            inputFileReader.Dispose();
        }
    }
}
