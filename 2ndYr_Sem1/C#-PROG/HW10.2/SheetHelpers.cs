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

        public static (int, int) ParseCellReference(ReadOnlySpan<char> cellReference)
        {
            int i = 0;
            while (i < cellReference.Length && cellReference[i] >= 'A' && cellReference[i] <= 'Z')
                i++;

            return (
                int.Parse(cellReference.Slice(i)),
                ConvertToColumnNumber(cellReference.Slice(0, i))
            );
        }

        public static bool TryParseCellReference(ReadOnlySpan<char> cellReference, out (int, int)? result)
        {
            try
            {
                result = ParseCellReference(cellReference);
                return true;
            }
            catch
            {
                result = (0, 0);
                return false;
            }
        }
    }
}
