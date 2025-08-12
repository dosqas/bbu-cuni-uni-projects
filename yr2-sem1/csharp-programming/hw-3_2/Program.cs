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

            try
            {
                inputValidator.ValidateCommandLineArguments(args);
            }
            catch (Exception e)
            {
                errHandler.HandleException(e);
            }

            string inputFile, outputFile, columnName;

            CLReader clReader = new CLReader(args);
            clReader.ReadArgs(out inputFile, out outputFile, out columnName);

            try
            {
                inputValidator.ValidateFile(inputFile);
            }
            catch (Exception e)
            {
                errHandler.HandleException(e);
            }

            char[] delimiters = { ' ', '\n', '\t', '\r' };
            using (StreamReader reader = new StreamReader(inputFile))
            {
                ITokenReader tokenReader = new TokenReaderByLine(reader, delimiters);
                TableSummarizer tableSummarizer = new TableSummarizer(tokenReader, columnName);

                try
                {
                    tableSummarizer.ProcessAllTokens();
                }
                catch (Exception e)
                {
                    errHandler.HandleException(e);
                }

                try
                {
                    using (StreamWriter writer = new StreamWriter(outputFile))
                    {
                        tableSummarizer.WriteReport(writer);
                    }
                }
                catch (Exception)
                {
                    errHandler.HandleException(new FormatException("File Error"));
                }
            }
        }
    }
}
