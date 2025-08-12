using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    internal class TreePrinter
    {
        public static void PrintTreePrefix(Node root)
        {
            if (root.Symbol == null) Console.Write(root.Weight.ToString() + ' '); 
            else Console.Write('*' + root.Symbol.ToString() + ':' + root.Weight.ToString() + ' ');

            if (root.Left != null)  PrintTreePrefix(root.Left);
            if (root.Right != null) PrintTreePrefix(root.Right);
        }
    }
}
