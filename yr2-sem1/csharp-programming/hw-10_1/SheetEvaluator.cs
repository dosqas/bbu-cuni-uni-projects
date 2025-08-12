using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace ExcelSheetEvaluator
{
    internal class SheetEvaluator
    {
        private static readonly Dictionary<(int, int), CellInfo> cellCache = new();

        private class CellInfo
        {
            public string RawValue;
            public int? EvaluatedValue;
            public bool IsProcessing;
            public bool IsEmpty;
            public (int, int)? FirstDependency;
            public (int, int)? SecondDependency;
            public char? Operator;
        }

        public static void EvaluateSheet()
        {
            foreach (var kvp in Sheet.Cells)
            {
                var cellValue = kvp.Value;
                var cellInfo = new CellInfo { RawValue = cellValue };

                if (cellValue == "[]")
                {
                    cellInfo.IsEmpty = true;
                    cellInfo.EvaluatedValue = 0;
                }
                else if (cellValue.Length > 0 && cellValue[0] == '=')
                {
                    ReadOnlySpan<char> formula = cellValue.AsSpan(1);
                    int opIndex = formula.IndexOfAny("+-*/");

                    if (opIndex != -1)
                    {
                        cellInfo.FirstDependency = SheetHelpers.ParseCellReference(formula.Slice(0, opIndex));
                        cellInfo.SecondDependency = SheetHelpers.ParseCellReference(formula.Slice(opIndex + 1));
                        cellInfo.Operator = formula[opIndex];
                    }
                }
                else
                {
                    cellInfo.EvaluatedValue = int.Parse(cellValue);
                }

                cellCache[kvp.Key] = cellInfo;
            }

            foreach (var key in cellCache.Keys)
            {
                EvaluateCell(key);
            }

            foreach (var kvp in cellCache)
            {
                Sheet.Cells[kvp.Key] = kvp.Value.IsEmpty ? "[]" : kvp.Value.EvaluatedValue.ToString();
            }
        }

        private static int EvaluateCell((int, int) key)
        {
            var cellInfo = cellCache[key];

            if (cellInfo.EvaluatedValue.HasValue)
                return cellInfo.EvaluatedValue.Value;

            if (cellInfo.IsProcessing)
                throw new InvalidOperationException("Circular reference detected");

            if (cellInfo.FirstDependency == null)
                throw new InvalidOperationException("Invalid cell state");

            cellInfo.IsProcessing = true;

            int firstValue = EvaluateCell(cellInfo.FirstDependency.Value);
            int secondValue = EvaluateCell(cellInfo.SecondDependency.Value);

            int result = cellInfo.Operator switch
            {
                '+' => firstValue + secondValue,
                '-' => firstValue - secondValue,
                '*' => firstValue * secondValue,
                '/' => firstValue / secondValue,
                _ => throw new InvalidOperationException("Invalid operator")
            };

            cellInfo.IsProcessing = false;
            cellInfo.EvaluatedValue = result;
            return result;
        }
    }
}