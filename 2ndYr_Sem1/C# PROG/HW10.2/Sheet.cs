using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExcelSheetEvaluator
{
    internal class Sheet
    {
        // (int, int) represents row and column; string represents the value stored in the cell
        public static SortedDictionary<(int, int), string> Cells = new SortedDictionary<(int, int), string>();

        public static void ChangeCellValue(int row, int column, string value)
        {
            Cells[(row, column)] = value;
        }
    }
}
