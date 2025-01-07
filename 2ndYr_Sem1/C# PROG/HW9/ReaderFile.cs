#nullable enable

using System;
using System.IO;

namespace HuffmanTree
{
    public class ReaderFile
    {
        public static NodeArray ReadFileToArray(String fileName)
        {
            try
            {
                NodeArray nodeArray = new NodeArray();
                using (FileStream reader = new FileStream(fileName, FileMode.Open, FileAccess.Read))
                {
                    int readByte;
                    while ((readByte = reader.ReadByte()) != -1)
                    {
                        nodeArray.Add(readByte);
                    }
                }
                return nodeArray;
            }
            catch
            {
                throw new IOException("File Error");
            }
        }
    }
}
