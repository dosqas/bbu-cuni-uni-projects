using cs_assignment3;
using System;
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
        private bool _nonEmptyLineDetected;
        private bool _fileEnded;

        public TokenReaderByChar(TextReader reader, char[] tokenDelimiters)
        {
            _charReader = reader;
            _tokenDelimiters = tokenDelimiters;
            _currentWord = null;
            _newLineDetected = true;
            _nonEmptyLineDetected = false;
            _fileEnded = false;
        }

        public Token ReadToken()
        {
            if (_fileEnded)
            {
                return new Token(Token.TokenType.EOF, null);
            }

            // Track if we've detected any words on the current line.
            bool wordDetected = false;

            while (true)
            {
                int readCharInt = _charReader.Read();
                if (readCharInt == -1)
                {
                    _fileEnded = true;

                    // If we ended on an empty line, emit EOP; otherwise, emit EOF.
                    return _newLineDetected && !wordDetected
                        ? new Token(Token.TokenType.EOP, null)
                        : new Token(Token.TokenType.EOF, null);
                }

                char readChar = (char)readCharInt;

                // Check if the character is a newline.
                if (readChar == '\n')
                {
                    if (_newLineDetected && !wordDetected)
                    {
                        // Two consecutive newlines with no words mean an empty line (EOP).
                        _newLineDetected = false;
                        return new Token(Token.TokenType.EOP, null);
                    }

                    // Mark that we’ve encountered a newline.
                    _newLineDetected = true;

                    // If words were detected on this line, return EOL.
                    if (wordDetected)
                    {
                        return new Token(Token.TokenType.EOL, null);
                    }

                    // Otherwise, continue to next character to check for empty line.
                    continue;
                }

                // If the character is a delimiter (not newline), continue to next character.
                if (_tokenDelimiters.Contains(readChar))
                {
                    continue;
                }

                // We've encountered a word character, so we start building a word.
                _newLineDetected = false;
                wordDetected = true;

                // Build the word until we encounter a delimiter.
                _currentWord = "";
                do
                {
                    _currentWord += readChar;
                    readCharInt = _charReader.Read();
                    if (readCharInt == -1)
                    {
                        _fileEnded = true;
                        return new Token(Token.TokenType.WORD, _currentWord);
                    }
                    readChar = (char)readCharInt;
                } while (!_tokenDelimiters.Contains(readChar) && readChar != '\n');

                // Push the last character back if it’s a newline, so we handle EOL after word emission.
                if (readChar == '\n')
                {
                    _newLineDetected = true;
                }

                return new Token(Token.TokenType.WORD, _currentWord);
            }
        }

    }
}
