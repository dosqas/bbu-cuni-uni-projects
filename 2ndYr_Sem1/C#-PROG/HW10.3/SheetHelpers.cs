using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ExcelSheetEvaluator
{
    internal class SheetHelpers
    {
        public static int ConvertToColumnNumber(ReadOnlySpan<char> columnTitle)
        {
            int col = 0;
            for (int i = 0; i < columnTitle.Length; i++)
            {
                col = col * 26 + (columnTitle[i] - 'A' + 1);
            }
            return col;
        }

        public static (string, int, int) ParseCellReference(ReadOnlySpan<char> cellReference, string sheetName)
        {
            int i = 0;
            while (i < cellReference.Length && cellReference[i] >= 'A' && cellReference[i] <= 'Z')
                i++;

            int row = int.Parse(cellReference.Slice(i));
            int column = ConvertToColumnNumber(cellReference.Slice(0, i));

            return (sheetName, row, column);
        }

        public static bool TryParseCellReference(ReadOnlySpan<char> cellReference, string sheetName, out (string, int, int)? result)
        {
            try
            {
                result = ParseCellReference(cellReference, sheetName);
                return true;
            }
            catch
            {
                result = null;
                return false;
            }
        }


    }
}