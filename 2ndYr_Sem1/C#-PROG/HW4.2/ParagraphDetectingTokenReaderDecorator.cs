using System;

namespace TokenProcessingFramework;

#nullable enable

public record class ParagraphDetectingTokenReaderDecorator(ITokenReader Reader) : ITokenReader
{
    private Token? _nextToken = null;
    private bool _detectedWordInParagraph = false;

    public Token ReadToken()
    {
        if (_nextToken is not null)
        {
            var tokenToReturn = _nextToken.Value;
            _nextToken = null;
            _detectedWordInParagraph = true;
            return tokenToReturn;
        }

        int newLinesFound = 0;
        Token token;

        while ((token = Reader.ReadToken()).Type == TokenType.EndOfLine)
        {
            newLinesFound++;
        }

        if (token.Type == TokenType.EndOfInput)
        {
            if (_detectedWordInParagraph)
            {
                _detectedWordInParagraph = false;
                return new Token(TokenType.EndOfParagraph);
            }
            return token; 
        }

        if (newLinesFound > 1)
        {
            if (_detectedWordInParagraph)
            {
                _nextToken = token;
                _detectedWordInParagraph = false;
                return new Token(TokenType.EndOfParagraph);
            }
        }

        if (token.Type == TokenType.Word)
        {
            _detectedWordInParagraph = true;
        }

        return token;
    }
}
