/*namespace cs_assignment3
{
    public class UnitTest1
    {
        [Fact]
        public void Test1NoArguments()
        {
            // Arrange
            var inputValidator = new ValidatorInput();
            var args = new string[] { };

            // Act
            var result = inputValidator.ValidateCommandLineArguments(args);

            // Assert
            Assert.False(result);
        }

        [Fact]
        public void Test2TooManyArguments()
        {
            // Arrange
            var inputValidator = new ValidatorInput();
            var args = new string[] { "arg1", "arg2", "arg3", "arg4" };

            // Act
            var result = inputValidator.ValidateCommandLineArguments(args);

            // Assert
            Assert.False(result);
        }

        [Fact]
        public void Test3ReadingFromCL()
        {
            // Arrange
            var clReader = new CLReader(new string[] { "file.txt" });
            var inputFile = "";

            // Act
            clReader.ReadArgs(out inputFile);

            // Assert
            Assert.Equal("file.txt", inputFile);
        }

        [Fact]
        public void Test4BadFileName()
        {
            // Arrange
            var validateFile = new ValidatorInput();

            // Act
            var result = validateFile.ValidateFile("aVeryWeirdFile.whatever");

            // Assert
            Assert.False(result);
        }

        [Fact]
        public void Test5EmptyFile()
        {
            // Arrange
            var fileContent = "";
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("", sw.ToString());
            }
        }

        [Fact]
        public void Test6TwoParagraph()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm paragraph one.

                Hi! I'm paragraph number two!
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("4\r\n5\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test7TwoParagraphsThreeLines()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm paragraph one.

                Hi! I'm paragraph number two!
                And I'm a line!
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("4\r\n9\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test8MoreWhitespaces()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm             paragraph            one.

                        Hi!          I'm          paragraph   number two!
                And I'm   a            line!
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("4\r\n9\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test9OneParagraph()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm paragraph one.
                Hi! I'm a line!
                And I'm a line too!
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("13\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test10OneParagraphEmptyLines()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm paragraph one.
                Hi! I'm a line!
                And I'm a line too!




                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("13\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test11WhitespaceLines()
        {
            // Arrange
            var fileContent = """ 

                    

                Hi! I'm paragraph one.
                                           
                Hi! I'm paragraph number two!
                And I'm a line!

                    
                        
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("4\r\n9\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test12TwoParagraphWhitespaceLine()
        {
            // Arrange
            var fileContent = """ 
                Hi! I'm paragraph one.
                                    
                Hi! I'm paragraph number two!
                And I'm a line!
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("4\r\n9\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test13ShortWords()
        {
            // Arrange
            var fileContent = """ 
                        a a 
                            
                b b h
                a

                a b

                b a             b
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("2\r\n4\r\n2\r\n3\r\n", sw.ToString());
            }
        }

        [Fact]
        public void Test14OneWord()
        {
            // Arrange
            var fileContent = """ 
                a
                """;
            var reader = new StringReader(fileContent);
            var delims = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delims);
            var paragraphWordCounter = new ParagraphWordCounter(tokenReader);

            // Act
            paragraphWordCounter.ProcessAllTokens();

            // Assert
            using (var sw = new StringWriter())
            {
                Console.SetOut(sw);
                var outputWriter = new OutputWriterToSTDOUT();
                paragraphWordCounter.WriteReport(outputWriter);
                Assert.Equal("1\r\n", sw.ToString());
            }
        }
    }
}*/