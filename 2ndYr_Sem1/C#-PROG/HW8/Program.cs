#nullable enable

using System;
using System.IO;

namespace HuffmanTree
{
    class Program
    {
        public static void Main(string[] args)
        {
            String? fileName;

            if ((fileName = ReaderArgs.ReadArgument(args)) == null)
            {
                Console.WriteLine("Argument Error");
                return;
            }

            NodeArray nodeArray;
            try
            {
                nodeArray = ReaderFile.ReadFileToArray(fileName);
            }
            catch (IOException)
            {
                Console.WriteLine("File Error");
                return;
            }

            Node root = TreeBuilder.MakeHuffmanTree(nodeArray.GetPriorityQueueFromNodes());

            TreePrinter.PrintTreePrefix(root);
        }
    }
}