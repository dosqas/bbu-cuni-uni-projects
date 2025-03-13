using System;

#nullable enable

namespace cs_assignment3
{
    public class Token
    {
        // Token types:
        // WORD: A word token = non-delimiter string.
        // EOL: End of line token. Signaled by a \n character.
        // EOP: End of paragraph token. Signaled by a \n\n sequence.
        // (or a \n followed only by (*) delimiters and then a \n)
        // EOF: End of file token. Signaled by the end of the file. -1 returned by Read().
        public enum TokenType { WORD, EOL, EOP, EOF };

        public TokenType Type { get; init; }
        public string? Word { get; init; }

        public Token(TokenType tType, string? word)
        {
            // Check if the token type and the word match.
            // If the token type is WORD, the word must not be null.
            // If the token type is not WORD, the word must be null.
            if ((tType == TokenType.WORD && word == null) || (tType != TokenType.WORD && word != null))
            {
                throw new ArgumentException("The type of the token does not fit with the provided word.");
            }

            Type = tType;
            Word = word;
        }
    }
}
