/*using TokenProcessingFramework;
namespace TestTokenProcessingFramework;

#nullable enable

public class FakeTokenReader : ITokenReader
{
    Queue<Token> _tokens = new Queue<Token>();
    public FakeTokenReader(Queue<Token> tokens)
    {
        _tokens = tokens;
    }
    public Token ReadToken()
    {
        return _tokens.Count > 0 ? _tokens.Dequeue() : new Token(TokenType.EndOfInput);
    }
}
public class UnitTest1
{
    [Fact]
    public void EmptyInput()
    {
        // Arrange
        var tokens = new Queue<Token>();
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.EndOfInput, token.Type);
    }

    [Fact]
    public void OneParagraphOneLineNoWords()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.EndOfInput, token1.Type);
    }

    [Fact]
    public void OneParagraphOneLineOneWord()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.EndOfParagraph, token2.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token3.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void OneParagraphOneLineMultipleWords()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.EndOfParagraph, token4.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token5.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void OneParagraphMultipleLinesNoWords()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.EndOfInput, token1.Type);  // !!!!!!!!!!

        // Test fails at this line because it emits EndOfParagraph even though there are no words in the paragraph.
    }

    [Fact]
    public void OneParagraphMultipleLinesOneWordPerLine()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.EndOfParagraph, token4.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token5.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void OneParagraphMultipleLinesMultipleWordsPerLine()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "four"),
            new Token(TokenType.Word, "five"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "six"),
            new Token(TokenType.Word, "seven"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();
        var token8 = paragraphReader.ReadToken();
        var token9 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.Word, token4.Type);
        Assert.Equal(TokenType.Word, token5.Type);
        Assert.Equal(TokenType.Word, token6.Type);
        Assert.Equal(TokenType.Word, token7.Type);
        Assert.Equal(TokenType.EndOfParagraph, token8.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token9.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void MultipleParagraphsAndPerParagraphOneLineOneWord()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.EndOfParagraph, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.EndOfParagraph, token4.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token5.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void MultipleParagraphsAndPerParagraphOneLineMultipleWords()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.Word, "four"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.EndOfParagraph, token3.Type);
        Assert.Equal(TokenType.Word, token4.Type);
        Assert.Equal(TokenType.Word, token5.Type);
        Assert.Equal(TokenType.EndOfParagraph, token6.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token7.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void MultipleParagraphsAndPerParagraphMultipleLinesOneWordPerLine()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "four"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.EndOfParagraph, token3.Type);
        Assert.Equal(TokenType.Word, token4.Type);
        Assert.Equal(TokenType.Word, token5.Type);
        Assert.Equal(TokenType.EndOfParagraph, token6.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token7.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void MultipleParagraphsAndPerParagraphMultipleLinesMultipleWordsPerLine()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one"),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.Word, "four"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "five"),
            new Token(TokenType.Word, "six"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "seven"),
            new Token(TokenType.Word, "eight"),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();
        var token8 = paragraphReader.ReadToken();
        var token9 = paragraphReader.ReadToken();
        var token10 = paragraphReader.ReadToken();
        var token11 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.Word, token4.Type);
        Assert.Equal(TokenType.EndOfParagraph, token5.Type);
        Assert.Equal(TokenType.Word, token6.Type);
        Assert.Equal(TokenType.Word, token7.Type);
        Assert.Equal(TokenType.Word, token8.Type);
        Assert.Equal(TokenType.Word, token9.Type);
        Assert.Equal(TokenType.EndOfParagraph, token10.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token11.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void LinesAtStartAndEndMultipleParagraphsOneAndMultipleLinesOneOrMoreWordsPerLine()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "one"),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "five"),
            new Token(TokenType.Word, "six"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "seven"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();
        var token8 = paragraphReader.ReadToken();
        var token9 = paragraphReader.ReadToken();
        var token10 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.EndOfParagraph, token4.Type);
        Assert.Equal(TokenType.Word, token5.Type);
        Assert.Equal(TokenType.Word, token6.Type);
        Assert.Equal(TokenType.EndOfParagraph, token7.Type);
        Assert.Equal(TokenType.Word, token8.Type);
        Assert.Equal(TokenType.EndOfParagraph, token9.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token10.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void EndLineAtEnd()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "one"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "two"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "three"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.Word, "four"),
            new Token(TokenType.Word, "five"),
            new Token(TokenType.EndOfLine, null),
            new Token(TokenType.EndOfInput, null)
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();
        var token4 = paragraphReader.ReadToken();
        var token5 = paragraphReader.ReadToken();
        var token6 = paragraphReader.ReadToken();
        var token7 = paragraphReader.ReadToken();
        var token8 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.Word, token2.Type);
        Assert.Equal(TokenType.Word, token3.Type);
        Assert.Equal(TokenType.EndOfParagraph, token4.Type);
        Assert.Equal(TokenType.Word, token5.Type);
        Assert.Equal(TokenType.Word, token6.Type);
        Assert.Equal(TokenType.EndOfParagraph, token7.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token8.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }

    [Fact]
    public void NotProperlyEnded()
    {
        // Arrange
        var tokens = new Queue<Token>(new[] {
            new Token(TokenType.Word, "one")
        });
        var reader = new FakeTokenReader(tokens);
        var paragraphReader = new ParagraphDetectingTokenReaderDecorator(reader);

        // Act
        var token1 = paragraphReader.ReadToken();
        var token2 = paragraphReader.ReadToken();
        var token3 = paragraphReader.ReadToken();

        // Assert
        Assert.Equal(TokenType.Word, token1.Type);
        Assert.Equal(TokenType.EndOfParagraph, token2.Type);  // !!!!!!!!!!
        Assert.Equal(TokenType.EndOfInput, token3.Type);

        // Test fails at this line because it does not emit EndOfParagraph token before the EndOfInput token.
    }
}*/