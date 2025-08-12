namespace cs_assignment3
{
    // Our ParagraphWordCounter class implements the ITokenProcessor interface.
    // This class is responsible for counting the number of words in each paragraph.
    public class ParagraphWordCounter : ITokenProcessor
    {
        private int _currentParagraphIndex;
        private int[] _paragraphCounts;
        private ITokenReader _tokenReader;

        public ParagraphWordCounter(ITokenReader tokenReader)
        {
            _currentParagraphIndex = 0;
            // Should make the array dynamically allocated.
            // 10000 works well enough though.
            _paragraphCounts = new int[10000];
            _tokenReader = tokenReader;
        }

        public void ProcessAllTokens()
        {
            Token currentToken = _tokenReader.ReadToken();

            while (currentToken.Type != Token.TokenType.EOF)
            {
                // We read until we reach the end of the file.
                if (currentToken.Type == Token.TokenType.EOP && _paragraphCounts[_currentParagraphIndex] != 0)
                {
                    // If we reach the end of the paragraph, we move to the next one.
                    // We stop counting the words in the current paragraph.
                    _currentParagraphIndex++;
                }
                else if (currentToken.Type == Token.TokenType.WORD)
                {
                    _paragraphCounts[_currentParagraphIndex]++;
                }

                currentToken = _tokenReader.ReadToken();
            }
        }

        public void WriteReport(IOutputWriter textWriter)
        {
            if (_paragraphCounts.Length > 0)
            {
                // Ensures that we don't print out empty paragraphs.
                for (int i = 0; i <= _currentParagraphIndex; i++)
                {
                    if (_paragraphCounts[i] == 0)
                        continue;
                    textWriter.WriteOutput(_paragraphCounts[i]);
                }
            }
        }
    }
}
