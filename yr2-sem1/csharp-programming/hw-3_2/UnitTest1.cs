/*namespace cs_assignment3
{
    public class UnitTest1
    {
        [Fact]
        public void Test1NoArgs()
        {
            // Arrange
            var inputValidator = new ValidatorInput();
            var args = new string[] { };

            // Act
            try
            {
                inputValidator.ValidateCommandLineArguments(args);
            }
            catch (System.ArgumentException e)
            {
                // Assert
                Assert.Equal("Argument Error", e.Message);
            }
        }

        [Fact]
        public void Test2ALotOfArgs()
        {
            // Arrange
            var inputValidator = new ValidatorInput();
            var args = new string[] { "arg1", "arg2", "arg3", "arg4", "arg5" };

            // Act
            try
            {
                inputValidator.ValidateCommandLineArguments(args);
            }
            catch (System.ArgumentException e)
            {
                // Assert
                Assert.Equal("Argument Error", e.Message);
            }
        }

        [Fact]
        public void Test3CommandLineReading()
        {
            // Arrange
            var clReader = new CLReader(new string[] { "input.txt", "output.txt", "column" });
            string inputFile, outputFile, columnName;

            // Act
            clReader.ReadArgs(out inputFile, out outputFile, out columnName);

            // Assert
            Assert.Equal("input.txt", inputFile);
            Assert.Equal("output.txt", outputFile);
            Assert.Equal("column", columnName);
        }

        [Fact]
        public void Test4WeirdFileName()
        {
            // Arrange
            var inputValidator = new ValidatorInput();
            var filename = "thisfiledoesnotexist.haha";

            // Act
            try
            {
                inputValidator.ValidateFile(filename);
            }
            catch (System.IO.IOException e)
            {
                // Assert
                Assert.Equal("File Error", e.Message);
            }
        }

        [Fact]
        public void Test5EmptyFile()
        {
            // Arrange
            var fileContent = """

                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.InvalidOperationException e) {
                // Assert
                Assert.Equal("Non-existent Column Name", e.Message);
            }
        }

        [Fact]
        public void Test6ColumnNotFound()
        {
            // Arrange
            var fileContent = """
                column1 column2 column3
                1 2 3
                4 5 6
                7 8 9
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column4");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.InvalidOperationException e)
            {
                // Assert
                Assert.Equal("Non-existent Column Name", e.Message);
            }
        }

        [Fact]
        public void Test7TooFewColumns()
        {
            // Arrange
            var fileContent = """
                column1 column2 column3
                1 2 3
                4 5 6
                7 8
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.FormatException e)
            {
                // Assert
                Assert.Equal("Invalid File Format", e.Message);
            }
        }

        [Fact]
        public void Test8TooManyColumns()
        {
            // Arrange
            var fileContent = """
                column1      column2      column3 
                1 2   3
                4 5     6
                7 8 9 10 11

                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.FormatException e)
            {
                // Assert
                Assert.Equal("Invalid File Format", e.Message);
            }
        }

        [Fact]
        public void Test9EmptyLine()
        {
            // Arrange
            var fileContent = """
                column1 column2 column3
                1 2 3

                4 5 6
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.FormatException e)
            {
                // Assert
                Assert.Equal("Invalid File Format", e.Message);
            }
        }

        [Fact]
        public void Test10WeirdValue()
        {
            // Arrange
            var fileContent = """
                column1 column2 column3
                1 2 3
                4 5 6
                7 8 notanumber
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (System.FormatException e)
            {
                // Assert
                Assert.Equal("Invalid Integer Value", e.Message);
            }
        }

        [Fact]
        public void Test11NormalCase()
        {
            // Arrange
            var fileContent = """
                column1 column2 column3
                1 2 3
                4 5 6
                7 8 9
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");
            var result = "";

            // Act
            using (var memoryStream = new MemoryStream())
            using (var streamWriter = new StreamWriter(memoryStream))
            {
                tableSummarizer.ProcessAllTokens();
                tableSummarizer.WriteReport(streamWriter);
                streamWriter.Flush(); 
                memoryStream.Position = 0; 
                using (var readerForResult = new StreamReader(memoryStream))
                {
                    result = readerForResult.ReadToEnd();
                }
            }

            // Assert
            Assert.Equal("column3\r\n-------\r\n18\r\n", result);
        }

        [Fact]
        public void Test12Whitespaces()
        {
            // Arrange
            var fileContent = """
                    column1      column2         column3
                1            2                     3
                         4       5          6
                     7               8      9
                
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");
            var result = "";

            // Act
            using (var memoryStream = new MemoryStream())
            using (var streamWriter = new StreamWriter(memoryStream))
            {
                tableSummarizer.ProcessAllTokens();
                tableSummarizer.WriteReport(streamWriter);
                streamWriter.Flush();
                memoryStream.Position = 0;
                using (var readerForResult = new StreamReader(memoryStream))
                {
                    result = readerForResult.ReadToEnd();
                }
            }

            // Assert
            Assert.Equal("column3\r\n-------\r\n18\r\n", result);
        }

        [Fact]
        public void Test13EmptyFirstLine()
        {
            // Arrange
            var fileContent = """
               
                1 2 3
                4 5 6
                """;
            var reader = new StringReader(fileContent);
            var delimiters = new char[] { ' ', '\n', '\t', '\r' };
            var tokenReader = new TokenReaderByLine(reader, delimiters);
            var tableSummarizer = new TableSummarizer(tokenReader, "column3");

            // Act
            try
            {
                tableSummarizer.ProcessAllTokens();
            }
            catch (Exception e)
            {
                // Assert
                Assert.Equal("Invalid File Format", e.Message);
            }
        }
    }
}*/