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
        private TextReader _lineReader;
        private char[] _tokenDelimiters;
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

            if (_currentWordIndex == null)
            {
                // Means we have no line loaded.
                string? line = _lineReader.ReadLine();

                if (line == null)
                {
                    // Means we have reached the end of the file.
                    return new Token(Token.TokenType.EOF, null);
                }

                _currentLineWords = line?.Split(_tokenDelimiters, StringSplitOptions.RemoveEmptyEntries);

                if (_currentLineWords != null && _currentLineWords.Length == 0)
                {
                    // Means the line has no words in it.
                    return new Token(Token.TokenType.EOP, null);
                }

                _currentWordIndex = 0;
            }

            if (_currentLineWords != null && _currentWordIndex == _currentLineWords.Length)
            {
                // Means we have reached the end of the current line.
                _currentWordIndex = null;
                return new Token(Token.TokenType.EOL, null);
            }

            if (_currentLineWords != null)
                // We return the next word in the current line.
                return new Token(Token.TokenType.WORD, _currentLineWords[(int)_currentWordIndex++]);
            else throw new InvalidOperationException("WTF?");
        }
    }
}
