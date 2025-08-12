using System;
using System.ComponentModel;
using System.IO;
using System.Linq;

#nullable enable

namespace cs_assignment3
{
    // This class reads tokens from a TextReader by reading one character at a time.
    public class TokenReaderByChar : ITokenReader
    {
        private TextReader _charReader;
        private char[] _tokenDelimiters;
        private string? _currentWord;
        private bool _newLineDetected;
        private bool _fileEnded;

        public TokenReaderByChar(TextReader reader, char[] tokenDelimiters)
        {
            _charReader = reader;
            _tokenDelimiters = tokenDelimiters;
            _currentWord = null;
            _newLineDetected = true;
            _fileEnded = false;
        }

        public Token ReadToken()
        {
            if (_fileEnded)
            {
                return new Token(Token.TokenType.EOF, null);
            }

            bool wordDetected = false;
            _currentWord = "";

            while (true)
            {
                int readCharInt = _charReader.Read();
                if (readCharInt == -1)
                {
                    _fileEnded = true;

                    // Emit `EOP` if no word detected and a new line was expected, else emit `EOF`.
                    return _newLineDetected && !wordDetected
                        ? new Token(Token.TokenType.EOP, null)
                        : new Token(Token.TokenType.EOF, null);
                }

                char readChar = (char)readCharInt;

                if (readChar == '\n')
                {
                    // If `\n` was detected after another `\n`, emit `EOP`.
                    if (_newLineDetected && !wordDetected)
                    {
                        _newLineDetected = false;
                        return new Token(Token.TokenType.EOP, null);
                    }

                    // Mark that a newline was detected.
                    _newLineDetected = true;

                    // If we previously detected a word, emit `EOL`.
                    if (wordDetected)
                    {
                        wordDetected = false; // Reset for next line
                        return new Token(Token.TokenType.EOL, null);
                    }

                    // Otherwise, continue to the next character.
                    continue;
                }

                // If it's a delimiter, check if we have accumulated a word and emit it as a `WORD` token.
                if (Array.IndexOf(_tokenDelimiters, readChar) >= 0)
                {
                    if (_currentWord.Length > 0)
                    {
                        var wordToken = new Token(Token.TokenType.WORD, _currentWord);
                        _currentWord = "";  // Reset for next word
                        return wordToken;
                    }
                    continue;
                }

                // We've found a character that’s part of a word.
                _newLineDetected = false;
                wordDetected = true;
                _currentWord += readChar; // Accumulate characters for the current word
            }
        }

        public void SkipRest()
        {
            return;
        }
    }
}
