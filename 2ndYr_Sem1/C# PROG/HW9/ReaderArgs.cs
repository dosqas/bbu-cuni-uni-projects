#nullable enable

using System;

namespace HuffmanTree
{
    public class ReaderArgs
    {
        public static String? ReadArgument(String[] args)
        {
            if (args.Length != 1) return null;

            return args[0];
        }
    }
}
