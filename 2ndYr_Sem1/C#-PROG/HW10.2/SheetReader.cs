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
                string[] lines = File.ReadAllLines(inputFileName);
                int row = 1;

                foreach (string line in lines)
                {
                    int startIndex = 0;
                    int column = 0;
                    int length = line.Length;

                    while (startIndex < length && line[startIndex] == ' ')
                    {
                        startIndex++;
                    }

                    for (int i = startIndex; i <= length; i++)
                    {
                        if (i == length || line[i] == ' ')
                        {
                            if (i > startIndex)
                            {
                                column++;
                                Sheet.ChangeCellValue(row, column, line.Substring(startIndex, i - startIndex));
                            }
                            while (i < length && line[i] == ' ')
                            {
                                i++;
                            }
                            startIndex = i;
                        }
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