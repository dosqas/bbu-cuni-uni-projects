using System.Collections.Generic;
using System;
using System.IO;
using System.Linq;

namespace ExcelSheetEvaluator
{
    internal class SheetEvaluator
    {
        public class CellInfo
        {
            public string Value;
            public (string, int, int)? CellReference;
            public (string, int, int)? FirstRef;
            public (string, int, int)? SecondRef;
            public char? Operator;
            public string ErrorCode;
            public bool Evaluated;
            public int? ComputedValue;
        }


        public static readonly Dictionary<(int, int), string> SheetCells = new(20);
        private static readonly Dictionary<(string, int, int), CellInfo> cells = new(20);
        private static readonly HashSet<(string, int, int)> processing = new(20);
        private static readonly HashSet<(string, int, int)> cyclic = new(20);
        private static readonly HashSet<string> loadedSheets = new(10, StringComparer.Ordinal);
        private static readonly char[] operatorChars = { '+', '-', '*', '/' };
        public static string SheetName;

        private static bool ParseFormula(ReadOnlySpan<char> formula, string sheetName, out (string, int, int)? firstRef, out (string, int, int)? secondRef, out char? op)
        {
            firstRef = secondRef = null;
            op = null;

            int opIndex = formula.IndexOfAny(operatorChars);
            if (opIndex == -1) return false;

            op = formula[opIndex];
            var firstPart = formula.Slice(0, opIndex);
            var secondPart = formula.Slice(opIndex + 1);

            if (firstPart.IsEmpty || secondPart.IsEmpty) return false;

            return (firstPart.Contains('!') ? ParseExtReference(firstPart, out firstRef) :
                   SheetHelpers.TryParseCellReference(firstPart, sheetName, out firstRef)) &&
                   (secondPart.Contains('!') ? ParseExtReference(secondPart, out secondRef) :
                   SheetHelpers.TryParseCellReference(secondPart, sheetName, out secondRef));
        }

        private static bool ParseExtReference(ReadOnlySpan<char> reference, out (string, int, int)? cellRef)
        {
            cellRef = null;
            int exclamIndex = reference.IndexOf('!');
            if (exclamIndex <= 0 || exclamIndex >= reference.Length - 1) return false;

            var sheetName = reference.Slice(0, exclamIndex).ToString();
            return LoadSheet(sheetName) &&
                   SheetHelpers.TryParseCellReference(reference.Slice(exclamIndex + 1), sheetName, out cellRef);
        }

        public static void EvaluateSheet(string outputFileName)
        {
            string currentSheetName = SheetName;
            loadedSheets.Add(currentSheetName);

            foreach (var kvp in SheetCells)
            {
                var cellValue = kvp.Value;
                var cell = new CellInfo { Value = cellValue };
                var currentKey = (currentSheetName, kvp.Key.Item1, kvp.Key.Item2);

                if (cellValue != "[]" && !cellValue.StartsWith("=") && !cellValue.Contains('!'))
                {
                    if (int.TryParse(cellValue, out int value))
                    {
                        cell.ComputedValue = value;
                        cell.Evaluated = true;
                    }
                    else
                    {
                        cell.ErrorCode = "#INVVAL";
                    }
                }
                else if (cellValue.StartsWith("="))
                {
                    if (!ParseFormula(cellValue.AsSpan(1), currentSheetName, out cell.FirstRef, out cell.SecondRef, out cell.Operator))
                    {
                        cell.ErrorCode = cellValue.AsSpan(1).IndexOfAny(operatorChars) == -1 ? "#MISSOP" : "#FORMULA";
                    }
                }
                else if (cellValue.Contains('!'))
                {
                    if (!ParseExtReference(cellValue, out cell.CellReference))
                    {
                        cell.ErrorCode = "#ERROR";
                    }
                }

                cells[currentKey] = cell;
                SheetCells.Remove(kvp.Key);
            }

            WriteSheet(outputFileName, currentSheetName);
        }

        private static void WriteSheet(string outputFileName, string currentSheetName)
        {
            using (StreamWriter writer = new StreamWriter(outputFileName))
            {
                int lastRowValue = 1;
                bool wroteFirstCell = false;

                foreach (var kvp in cells.OrderBy(k => k.Key.Item2).ThenBy(k => k.Key.Item3))
                {
                    if (kvp.Key.Item1 == currentSheetName)
                    {
                        if ((kvp.Value.Operator.HasValue && kvp.Value.ErrorCode == null) || kvp.Value.CellReference != null)
                        {
                            EvaluateCell(kvp.Key);
                        }

                        if (lastRowValue != kvp.Key.Item2)
                        {
                            writer.Write('\n');
                            lastRowValue = kvp.Key.Item2;
                        }
                        else if (wroteFirstCell)
                        {
                            writer.Write(' ');
                        }

                        wroteFirstCell = true;
                        writer.Write(cyclic.Contains(kvp.Key) ? "#CYCLE" : (kvp.Value.ErrorCode ?? kvp.Value.Value));
                    }
                }
            }
        }

        private static bool LoadSheet(string sheetName)
        {
            if (loadedSheets.Contains(sheetName))
                return true;

            try
            {
                using var reader = new StreamReader($"{sheetName}.sheet");
                int row = 1;

                while (reader.ReadLine() is string line)
                {
                    ProcessSheetLine(line, sheetName, row++);
                }

                loadedSheets.Add(sheetName);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private static void ProcessSheetLine(string line, string sheetName, int row)
        {
            int startIndex = 0;
            int column = 0;
            int length = line.Length;

            ReadOnlySpan<char> trimmedLine = line.AsSpan().Trim();

            for (int i = 0; i <= trimmedLine.Length; i++)
            {
                if (i == trimmedLine.Length || trimmedLine[i] == ' ')
                {
                    if (i > startIndex)
                    {
                        column++;
                        ProcessCell(trimmedLine.Slice(startIndex, i - startIndex), sheetName, row, column);
                    }
                    while (i < trimmedLine.Length && trimmedLine[i] == ' ')
                        i++;
                    startIndex = i;
                }
            }
        }

        private static void ProcessCell(ReadOnlySpan<char> cellValue, string sheetName, int row, int column)
        {
            var cell = new CellInfo { Value = cellValue.ToString() };

            if (cellValue.StartsWith("="))
            {
                if (!ParseFormula(cellValue.Slice(1), sheetName, out cell.FirstRef, out cell.SecondRef, out cell.Operator))
                {
                    cell.ErrorCode = cellValue.Slice(1).IndexOfAny(operatorChars) == -1 ? "#MISSOP" : "#FORMULA";
                }
            }
            else if (cellValue.Contains('!'))
            {
                if (!ParseExtReference(cellValue, out cell.CellReference))
                {
                    cell.ErrorCode = "#ERROR";
                }
            }

            cells[(sheetName, row, column)] = cell;
        }

        private static (bool success, int value) EvaluateCell((string, int, int) key)
        {
            if (cyclic.Contains(key))
                return (false, 0);

            if (!cells.TryGetValue(key, out var cell))
                return (false, 0);

            if (cell.ErrorCode != null)
                return (false, 0);

            if (cell.Evaluated && cell.ComputedValue.HasValue)
                return (true, cell.ComputedValue.Value);

            if (!processing.Add(key))
            {
                MarkCycle(key);
                return (false, 0);
            }

            try
            {
                return EvaluateCellInternal(key, cell);
            }
            finally
            {
                processing.Remove(key);
            }
        }

        private static (bool success, int value) EvaluateCellInternal((string, int, int) key, CellInfo cell)
        {
            if (cell.ComputedValue.HasValue)
                return (true, cell.ComputedValue.Value);

            if (cell.Value == "[]")
                return (true, 0);

            if (cell.CellReference.HasValue)
            {
                var (success, refValue) = GetOperandValue(cell.CellReference);
                if (success)
                {
                    cell.ComputedValue = refValue;
                    cell.Value = refValue.ToString();
                    cell.Evaluated = true;
                }
                else
                {
                    cell.ErrorCode = "#ERROR";
                }
                return (success, refValue);
            }

            if (cell.FirstRef.HasValue && cell.SecondRef.HasValue && cell.Operator.HasValue)
            {
                var (firstSuccess, firstValue) = GetOperandValue(cell.FirstRef);
                if (!firstSuccess)
                {
                    cell.ErrorCode = "#ERROR";
                    return (false, 0);
                }

                var (secondSuccess, secondValue) = GetOperandValue(cell.SecondRef);
                if (!secondSuccess)
                {
                    cell.ErrorCode = "#ERROR";
                    return (false, 0);
                }

                if (cell.Operator == '/' && secondValue == 0)
                {
                    cell.ErrorCode = "#DIV0";
                    return (false, 0);
                }

                int result = cell.Operator switch
                {
                    '+' => firstValue + secondValue,
                    '-' => firstValue - secondValue,
                    '*' => firstValue * secondValue,
                    '/' => firstValue / secondValue,
                    _ => 0
                };

                cell.ComputedValue = result;
                cell.Value = result.ToString();
                cell.Evaluated = true;
                return (true, result);
            }

            if (int.TryParse(cell.Value, out int value))
            {
                cell.ComputedValue = value;
                cell.Evaluated = true;
                return (true, value);
            }

            return (false, 0);
        }

        private static void MarkCycle((string, int, int) key)
        {
            foreach (var cell in processing)
            {
                cyclic.Add(cell);
                cells[cell].ErrorCode = "#CYCLE";
            }
            processing.Clear();
        }

        private static (bool success, int value) GetOperandValue((string, int, int)? refCell)
        {
            if (!refCell.HasValue || !cells.TryGetValue(refCell.Value, out var cell))
                return (true, 0);

            if (cell.Value == "[]")
                return (true, 0);

            if (cyclic.Contains(refCell.Value))
                return (false, 0);

            if (cell.CellReference != null)
            {
                return EvaluateCell((cell.CellReference.Value.Item1, cell.CellReference.Value.Item2, cell.CellReference.Value.Item3));
            }

            if (!cell.Operator.HasValue)
                return cell.ErrorCode != null ? (false, 0) : (cell.ComputedValue.HasValue ? (true, cell.ComputedValue.Value) : (true, int.Parse(cell.Value)));

            return EvaluateCell(refCell.Value);
        }
    }
}