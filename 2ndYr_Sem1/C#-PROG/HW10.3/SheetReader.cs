using System;
using System.IO;
using System.Collections.Generic;

#nullable enable
namespace ExcelSheetEvaluator
{
    internal class SheetReader
    {
        public static void ReadSheet(string inputFileName)
        {
            try
            {
                using var reader = new StreamReader(inputFileName);
                int row = 1;

                while (reader.ReadLine() is string line)
                {
                    ReadOnlySpan<char> spanLine = line.AsSpan();
                    int column = 0;

                    while (!spanLine.IsEmpty)
                    {
                        spanLine = spanLine.TrimStart();
                        int nextSpace = spanLine.IndexOf(' ');

                        if (nextSpace == -1)
                        {
                            column++;
                            SheetEvaluator.SheetCells[(row, column)] = spanLine.ToString();
                            break;
                        }

                        if (nextSpace > 0)
                        {
                            column++;
                            SheetEvaluator.SheetCells[(row, column)] = spanLine.Slice(0, nextSpace).ToString();
                        }

                        spanLine = spanLine.Slice(nextSpace + 1);
                    }

                    row++;
                }
            }
            catch (Exception)
            {
                throw new Exception("File Error");
            }
        }
    }
}