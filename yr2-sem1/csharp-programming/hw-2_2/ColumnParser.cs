using cs_a1a2;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace c_a1a2
{
    internal class ColumnParser
    {
        private int columnNumber;
        private int headerColumnLength;
        public CalculateSum sum = new CalculateSum();
        public void Parse(string[] input) {
            this.sum.Add(int.Parse(input[this.columnNumber]));
        }

        public void ParseHeader(string[] header, string columnName)
        {
            this.headerColumnLength = header.Length;
            this.columnNumber = Array.IndexOf(header, columnName);
        }

        public int GetColumnNumber()
        {
            return this.columnNumber;
        }

        public int GetHeaderColumnLength()
        {
            return this.headerColumnLength;
        }
    }
}
