using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExcelSheetEvaluator
{
    internal class SheetWriter
    {
        public static void PrintSheet(string outputFileName)
        {
            try
            {
                using (StreamWriter writer = new StreamWriter(outputFileName))
                {
                    int lastRowValue = 1;

                    var enumerator = Sheet.Cells.GetEnumerator();
                    if (enumerator.MoveNext())
                    {
                        var firstCell = enumerator.Current;
                        writer.Write(firstCell.Value);
                        lastRowValue = firstCell.Key.Item1;
                    }

                    while (enumerator.MoveNext())
                    {
                        var cell = enumerator.Current;
                        if (lastRowValue != cell.Key.Item1)
                        {
                            writer.Write('\n');
                            lastRowValue = cell.Key.Item1;
                        }
                        else
                        {
                            writer.Write(' ');
                        }

                        writer.Write(cell.Value);
                    }
                }
            }
            catch (Exception)
            {
                throw new Exception("File Error");
            }
        }
    }
}
