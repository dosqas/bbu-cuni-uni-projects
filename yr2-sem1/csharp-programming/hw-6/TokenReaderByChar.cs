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
        private bool _multiFile;

        public TokenReaderByChar(TextReader reader, char[] tokenDelimiters, bool multiFile = false)
        {
            _charReader = reader;
            _tokenDelimiters = tokenDelimiters;
            _currentWord = null;
            _newLineDetected = true;
            _fileEnded = false;
            _multiFile = multiFile;
        }

        public TokenReaderByChar() { }

        public Token ReadToken()
        {
            if (_fileEnded)
            {
                return new Token(Token.TokenType.EOF, null);
            }

            _currentWord = "";

            while (true)
            {
                int readCharInt = _charReader.Read();
                if (readCharInt == -1)
                {
                    _fileEnded = true;

                    // Emit last word if present.
                    if (_currentWord.Length > 0)
                    {
                        return new Token(Token.TokenType.WORD, _currentWord);
                    }

                    // Emit `EOP` if the last line was empty, else `EOF`.
                    return _newLineDetected && !_multiFile
                        ? new Token(Token.TokenType.EOP, null)
                        : new Token(Token.TokenType.EOF, null);
                }

                char readChar = (char)readCharInt;

                if (readChar == '\n')
                {
                    // If `\n` follows another `\n`, it's an empty line, so emit `EOP`.
                    if (_newLineDetected)
                    {
                        _newLineDetected = false;
                        return new Token(Token.TokenType.EOP, null);
                    }

                    // Mark newline detected.
                    _newLineDetected = true;

                    // Emit current word if it was accumulated.
                    if (_currentWord.Length > 0)
                    {
                        var wordToken = new Token(Token.TokenType.WORD, _currentWord);
                        _currentWord = "";  // Reset for next word
                        return wordToken;
                    }

                    // Emit `EOL` for line break if no word.
                    return new Token(Token.TokenType.EOL, null);
                }

                // Handle other delimiters, emitting any accumulated word.
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

                // Add character to current word.
                _newLineDetected = false;
                _currentWord += readChar;
            }
        }

        public void SkipRest()
        {
            while (true)
            {
                int readCharInt = _charReader.Read();
                if (readCharInt == -1)
                {
                    _fileEnded = true;
                    return;
                }

                char readChar = (char)readCharInt;

                if (readChar == '\n')
                {
                    // Mark that a newline was detected and exit the method.
                    _newLineDetected = true;
                    return;
                }
            }
        }
    }
}
