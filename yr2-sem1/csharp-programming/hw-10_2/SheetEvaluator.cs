using System.Collections.Generic;
using System;

namespace ExcelSheetEvaluator
{
    internal class SheetEvaluator
    {
        private class CellInfo
        {
            public string Value;
            public (int, int)? FirstRef;
            public (int, int)? SecondRef;
            public char? Operator;
            public string ErrorCode;
            public bool Evaluated;
        }

        private static readonly Dictionary<(int, int), CellInfo> cells = new(100);
        private static readonly HashSet<(int, int)> processing = new(20);
        private static readonly HashSet<(int, int)> cyclic = new(20);

        private static bool ParseFormula(ReadOnlySpan<char> formula, out (int, int)? firstRef, out (int, int)? secondRef, out char? op)
        {
            firstRef = secondRef = null;
            op = null;

            int opIndex = formula.IndexOfAny("+-*/");
            if (opIndex == -1) return false;

            if (!SheetHelpers.TryParseCellReference(formula.Slice(0, opIndex), out firstRef) ||
                !SheetHelpers.TryParseCellReference(formula.Slice(opIndex + 1), out secondRef))
                return false;

            op = formula[opIndex];
            return true;
        }

        public static void EvaluateSheet()
        {
            cells.Clear();
            processing.Clear();
            cyclic.Clear();

            foreach (var kvp in Sheet.Cells)
            {
                var cell = new CellInfo { Value = kvp.Value };

                if (cell.Value.StartsWith("="))
                {
                    if (!ParseFormula(cell.Value.AsSpan(1), out cell.FirstRef, out cell.SecondRef, out cell.Operator))
                    {
                        cell.ErrorCode = cell.Value.AsSpan(1).IndexOfAny("+-*/") == -1 ? "#MISSOP" : "#FORMULA";
                    }
                }
                else if (cell.Value != "[]" && !int.TryParse(cell.Value, out _))
                {
                    cell.ErrorCode = "#INVVAL";
                }

                cells[kvp.Key] = cell;
            }

            foreach (var kvp in cells)
            {
                if (kvp.Value.Operator.HasValue && kvp.Value.ErrorCode == null)
                {
                    EvaluateCell(kvp.Key);
                }
            }

            foreach (var kvp in cells)
            {
                Sheet.Cells[kvp.Key] = cyclic.Contains(kvp.Key) ? "#CYCLE" : (kvp.Value.ErrorCode ?? kvp.Value.Value);
            }
        }

        private static (bool success, int value) EvaluateCell((int, int) key)
        {
            if (cyclic.Contains(key))
                return (false, 0);

            var cell = cells[key];

            if (cell.ErrorCode != null)
                return (false, 0);

            if (cell.Evaluated)
                return (true, int.Parse(cell.Value));

            if (!processing.Add(key))
            {
                MarkCycle(key);
                return (false, 0);
            }

            var (firstSuccess, firstValue) = GetOperandValue(cell.FirstRef);
            if (!firstSuccess)
            {
                cell.ErrorCode = "#ERROR";
                processing.Remove(key);
                return (false, 0);
            }

            var (secondSuccess, secondValue) = GetOperandValue(cell.SecondRef);
            if (!secondSuccess)
            {
                cell.ErrorCode = "#ERROR";
                processing.Remove(key);
                return (false, 0);
            }

            if (cell.Operator == '/' && secondValue == 0)
            {
                cell.ErrorCode = "#DIV0";
                processing.Remove(key);
                return (false, 0);
            }

            int result;
            switch (cell.Operator)
            {
                case '+': result = firstValue + secondValue; break;
                case '-': result = firstValue - secondValue; break;
                case '*': result = firstValue * secondValue; break;
                case '/': result = firstValue / secondValue; break;
                default:
                    cell.ErrorCode = "#ERROR";
                    processing.Remove(key);
                    return (false, 0);
            }

            cell.Value = result.ToString();
            cell.Evaluated = true;
            processing.Remove(key);
            return (true, result);
        }

        private static void MarkCycle((int, int) key)
        {
            foreach (var cell in processing)
            {
                cyclic.Add(cell);
                cells[cell].ErrorCode = "#CYCLE";
            }
            processing.Clear();
        }

        private static (bool success, int value) GetOperandValue((int, int)? refCell)
        {
            if (!refCell.HasValue || !cells.TryGetValue(refCell.Value, out var cell))
                return (true, 0);

            if (cell.Value == "[]")
                return (true, 0);

            if (cyclic.Contains(refCell.Value))
                return (false, 0);

            if (!cell.Operator.HasValue)
                return cell.ErrorCode != null ? (false, 0) : (true, int.Parse(cell.Value));

            return EvaluateCell(refCell.Value);
        }
    }
}
