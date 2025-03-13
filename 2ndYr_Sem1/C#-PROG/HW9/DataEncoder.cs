#nullable enable

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    internal class DataEncoder
    {
        private static Dictionary<byte, (ulong bitPattern, int bitLength)> data = new();

        public static void Add(byte symbol, BitStream encodedData)
        {
            ulong bitPattern = 0;
            int bitLength = encodedData.BitCount;
            for (int i = 0; i < bitLength; i++)
            {
                if ((encodedData.ToByteStream()[i / 8] & (1 << (i % 8))) != 0)
                {
                    bitPattern |= (1UL << i);
                }
            }

            data[symbol] = (bitPattern, bitLength);
        }

        public static (ulong bitPattern, int bitLength) Get(byte symbol)
        {
            return data[symbol];
        }
    }

}
