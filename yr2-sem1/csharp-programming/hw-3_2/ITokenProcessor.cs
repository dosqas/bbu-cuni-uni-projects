using System;
using System.IO;

namespace cs_assignment3
{
    public interface ITokenProcessor
    {
        public void ProcessAllTokens();
        public void WriteReport(StreamWriter textWriter);

    }
}
