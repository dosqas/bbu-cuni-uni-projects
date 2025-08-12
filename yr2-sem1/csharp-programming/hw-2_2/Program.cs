using c_a1a2;
using System;
using System.IO;
using System.Linq;

namespace cs_a1a2
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            InputValidator valid = new InputValidator();
            ErrHandler errHandle = new ErrHandler();

            bool validity = valid.ValidateCommandLineArguments(args, 3);
            if (!validity)
            {
                errHandle.HandleError("cl");
            }

            ReadFromCL readCl = new ReadFromCL();
            string inputFile = readCl.ReadArgs(args, 1);
            string outputFile = readCl.ReadArgs(args, 2);
            string columnName = readCl.ReadArgs(args, 3);

            validity = valid.ValidateFile(inputFile);
            if (!validity)
            {
                errHandle.HandleError("file");
            }

            ReadFromFile readFromFile = new ReadFromFile();
            SeparateStringInWords separateStringInWords = new SeparateStringInWords();
            ColumnParser columnParser = new ColumnParser();

            readFromFile.ReadFile(inputFile
            , readHeader =>
            {
                if (string.IsNullOrWhiteSpace(readHeader) || readHeader.All(c => char.IsWhiteSpace(c)))
                {
                    errHandle.HandleError("line");
                }
                string[] words = separateStringInWords.SeparatingString(readHeader);
                validity = valid.ValidateHeader(words, columnName);
                if (!validity)
                {
                    errHandle.HandleError("header");
                }
                columnParser.ParseHeader(words, columnName);
            }
            , readLine =>
            {
                if (string.IsNullOrWhiteSpace(readLine) || readLine.All(c => char.IsWhiteSpace(c)))
                {
                    errHandle.HandleError("line");
                }
                string[] words = separateStringInWords.SeparatingString(readLine);
                validity = valid.ValidateLine(words.Length, columnParser.GetHeaderColumnLength());
                if (!validity)
                {
                    errHandle.HandleError("line");
                }

                validity = valid.ValidateValue(words[columnParser.GetColumnNumber()]);
                if (!validity)
                {
                    errHandle.HandleError("value");
                }

                columnParser.Parse(words);
            });

            //validity = valid.ValidateFile(outputFile);
            //if (!validity)
            //{
            //    errHandle.HandleError("file");
            //}

            //WriteToFile writeToFile = new WriteToFile(outputFile);

            //writeToFile.WriteOutput(columnName);
            //writeToFile.WriteOutput(new string('-', columnName.Length));
            //writeToFile.WriteOutput(columnParser.sum.getSum());

            try
            {
                using(StreamWriter writer = new StreamWriter(outputFile))
                {
                    writer.WriteLine(columnName);
                    writer.WriteLine(new string('-', columnName.Length));
                    writer.WriteLine(columnParser.sum.getSum());
                }
            }
            catch (Exception)
            {
                errHandle.HandleError("file");
            }
        }
    }
}
