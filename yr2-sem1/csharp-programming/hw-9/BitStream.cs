#nullable enable

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    internal class BitStream
    {
        private byte[] buffer;
        private int bitCount;
        private int byteCapacity;

        public BitStream(int initialCapacity = 8)
        {
            buffer = new byte[initialCapacity];
            bitCount = 0;
            byteCapacity = initialCapacity;
        }

        public BitStream(BitStream other)
        {
            bitCount = other.bitCount;
            byteCapacity = other.byteCapacity;
            buffer = new byte[byteCapacity];
            Array.Copy(other.buffer, buffer, (bitCount + 7) / 8);
        }

        public void AddBit(bool bit)
        {
            if (bitCount / 8 >= byteCapacity)
            {
                byteCapacity *= 2;
                Array.Resize(ref buffer, byteCapacity);
            }

            if (bit)
            {
                buffer[bitCount / 8] |= (byte)(1 << (bitCount % 8));
            }

            bitCount++;
        }

        public BitStream AddBitStream(BitStream other)
        {
            int totalBits = other.bitCount;
            int byteIndex = 0;
            int bitOffset = 0;

            while (totalBits > 0)
            {
                int bitsToCopy = Math.Min(8 - bitOffset, totalBits);

                byte sourceByte = (byte)((other.buffer[byteIndex] >> bitOffset) & ((1 << bitsToCopy) - 1));

                for (int i = 0; i < bitsToCopy; i++)
                {
                    bool bit = (sourceByte & (1 << i)) != 0;
                    AddBit(bit);
                }

                totalBits -= bitsToCopy;
                bitOffset += bitsToCopy;

                if (bitOffset == 8)
                {
                    bitOffset = 0;
                    byteIndex++;
                }
            }

            return this;
        }


        public ReadOnlySpan<byte> ToByteStream()
        {
            int requiredBytes = (bitCount + 7) / 8;
            return buffer.AsSpan(0, requiredBytes);
        }

        public int BitCount => bitCount;
    }
}
