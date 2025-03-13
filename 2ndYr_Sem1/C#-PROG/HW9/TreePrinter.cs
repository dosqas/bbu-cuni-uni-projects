using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    internal class TreePrinter
    {
        public static void PrintTreePrefixConsole(Node root)
        {
            if (root.Symbol == null) Console.Write(root.Weight.ToString() + ' '); 
            else Console.Write('*' + root.Symbol.ToString() + ':' + root.Weight.ToString() + ' ');

            if (root.Left != null)  PrintTreePrefixConsole(root.Left);
            if (root.Right != null) PrintTreePrefixConsole(root.Right);
        }

        public static void PrintHuffmanDataFile(Node root, String fileName)
        {
            using FileStream outputFileStream = new FileStream(fileName + ".huff", FileMode.Create, FileAccess.Write);

            byte[] hexValues = new byte[] { 0x7B, 0x68, 0x75, 0x7C, 0x6D, 0x7D, 0x66, 0x66 }; // header
            outputFileStream.Write(hexValues, 0, hexValues.Length);


            int currOffset = 8;
            BitStream bitStream = new BitStream();
            TreePrinterHelpers.PrintEncodedTreeDescriptionAndPrepareData(root, outputFileStream, ref currOffset, bitStream); // encoded tree description

            for (int i = 0; i < 8; i++)
                outputFileStream.WriteByte(0); // description terminator


            using FileStream inputFileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            TreePrinterHelpers.PrintEncodedData(inputFileStream, outputFileStream, ref currOffset); // encoded data
        }
    }
}
