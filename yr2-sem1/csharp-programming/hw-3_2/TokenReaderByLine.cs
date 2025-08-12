using System;
using System.IO;

#nullable enable

namespace cs_assignment3
{
    // This class reads tokens from a TextReader by reading one line at a time.
    // We store the words of the current line in an array once we read a line
    // and then return the words one by one, as we keep calling ReadToken().
    public class TokenReaderByLine : ITokenReader
    {
        private readonly TextReader _lineReader;
        private readonly char[] _tokenDelimiters;
        private string[]? _currentLineWords;
        private int? _currentWordIndex;
        private bool _fileEnded;

        public TokenReaderByLine(TextReader reader, char[] tokenDelimiters)
        {
            _lineReader = reader;
            _tokenDelimiters = tokenDelimiters;
            _currentLineWords = null;
            _currentWordIndex = null;
            _fileEnded = false;
        }

        public Token ReadToken()
        {
            if (_fileEnded)
                return new Token(Token.TokenType.EOF, null);

            if (!_currentWordIndex.HasValue)
            {
                // Read a new line
                string? line = _lineReader.ReadLine();

                if (line == null)
                {
                    _fileEnded = true;
                    return new Token(Token.TokenType.EOF, null);
                }

                // Split the line into words using the delimiters
                _currentLineWords = line.Split(_tokenDelimiters, StringSplitOptions.RemoveEmptyEntries);
                if (_currentLineWords.Length == 0)
                    return new Token(Token.TokenType.EOP, null);

                _currentWordIndex = 0;
            }

            if (_currentLineWords != null && _currentWordIndex == _currentLineWords.Length)
            {
                // End of line reached
                _currentWordIndex = null;
                return new Token(Token.TokenType.EOL, null);
            }

            // Return the current word
            return new Token(Token.TokenType.WORD, _currentLineWords?[(int)_currentWordIndex++] ?? throw new InvalidOperationException("Unexpected null word"));
        }

        public void SkipRest() 
        { 
            _currentWordIndex = null;
            _currentLineWords = null; 
        }
    }
}
