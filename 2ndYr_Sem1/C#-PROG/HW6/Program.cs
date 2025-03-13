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
                inputValidator.ValidateCommandLineArgumentsMultiFile(args);
            }
            catch (Exception e)
            {
                errHandler.HandleException(e);
            }

            bool enableHighlights;
            string inputFile = "", outputFile;
            int maxTextWidth;

            CLReader clReader = new CLReader(args);
            clReader.ReadInitialArgsMultiFile(out enableHighlights, out outputFile, out maxTextWidth);

            using (StreamWriter writer = new StreamWriter(outputFile))
            {
                writer.Write(string.Empty);
            }

            char[] delimiters = { ' ', '\n', '\t', '\r' };

            try
            {
                bool isFileEmpty = true;

                using (StreamWriter writer = new StreamWriter(outputFile, append: true))
                {
                    ITokenReader mockReader = new TokenReaderByChar();
                    TextJustificator textJustificator = new TextJustificator(mockReader, maxTextWidth, writer, enableHighlights);
                    do
                    {
                        bool resultOfTryingToRead = true;
                        while (true)
                        {
                            try
                            {
                                resultOfTryingToRead = clReader.ReadNextInputFileArgMultiFile(out inputFile);

                                inputValidator.ValidateFile(inputFile);
                                break;
                            }
                            catch (Exception)
                            {
                                if (!resultOfTryingToRead) break;
                            }
                        }

                        if (!resultOfTryingToRead) break;

                        using (StreamReader reader = new StreamReader(inputFile))
                        {
                            ITokenReader tokenReader = new TokenReaderByChar(reader, delimiters, multiFile: true);
                            textJustificator.SetReader(tokenReader);

                            textJustificator.ProcessAllTokensMultiFile();
                            isFileEmpty = false;
                        }
                    } while (true);

                    textJustificator.AllFilesProcessedRoutineMultiFile();
                }

                if (isFileEmpty)
                {
                    using (StreamWriter writer = new StreamWriter(outputFile, append: true))
                    {
                        writer.Write('\n');
                    }
                }
            }
            catch (Exception e)
            {
                errHandler.HandleException(e);
            }
        }
    }
}
