using System;
using System.IO;

namespace ExcelSheetEvaluator
{
    public class Program
    {
        public static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("Argument Error");
                return;
            }

            string inputFileName = args[0];
            string outputFileName = args[1];

            try
            {
                SheetReader.ReadSheet(inputFileName);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return;
            }

            SheetEvaluator.EvaluateSheet();

            try
            {
                SheetWriter.PrintSheet(outputFileName);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return;
            }
        }
    }
}
