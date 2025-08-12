#nullable enable

using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    internal class TreePrinterHelpers
    {
        private static byte[] PrepareNode(Node node)
        {
            byte[] result = new byte[8];
            ulong weightBits = (ulong)node.Weight & 0x007FFFFFFFFFFFFF;

            weightBits <<= 1;
            if (node.Symbol != null) { weightBits += 1; result[7] = (byte)node.Symbol; }

            for (int i = 0; i < 7; i++)
            {
                result[i] = (byte)(weightBits & 0xFF);
                weightBits >>= 8;
            }

            return result;
        }

        public static void PrintEncodedTreeDescriptionAndPrepareData(Node root, FileStream outputFileStream, ref int currOffset, BitStream bitStream)
        {
            byte[] nodeBytes = PrepareNode(root);
            outputFileStream.Write(nodeBytes, 0, nodeBytes.Length);
            currOffset += nodeBytes.Length;

            if (root.Symbol != null)
            {
                DataEncoder.Add((byte)root.Symbol, new BitStream(bitStream));
                return;
            }

            if (root.Left != null)
            {
                BitStream leftBitStream = new BitStream(bitStream);
                leftBitStream.AddBit(false);
                PrintEncodedTreeDescriptionAndPrepareData(root.Left, outputFileStream, ref currOffset, leftBitStream);
            }

            if (root.Right != null)
            {
                BitStream rightBitStream = new BitStream(bitStream);
                rightBitStream.AddBit(true);
                PrintEncodedTreeDescriptionAndPrepareData(root.Right, outputFileStream, ref currOffset, rightBitStream);
            }
        }

        public static void PrintEncodedData(FileStream inputFileStream, FileStream outputFileStream, ref int currOffset)
        {
            BitStream masterBitStream = new BitStream();

            int readByte;
            while ((readByte = inputFileStream.ReadByte()) != -1)
            {
                var (bitPattern, bitLength) = DataEncoder.Get((byte)readByte);

                for (int i = 0; i < bitLength; i++)
                {
                    bool bit = (bitPattern & (1UL << i)) != 0;
                    masterBitStream.AddBit(bit);
                }
            }

            byte[] bitStreamBytes = masterBitStream.ToByteStream().ToArray();
            outputFileStream.Write(bitStreamBytes, 0, bitStreamBytes.Length);
            currOffset += bitStreamBytes.Length;
        }

    }
}
