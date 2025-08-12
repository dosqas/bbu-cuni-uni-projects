/*using TokenProcessingFramework;

using TableSummatorApp;
using System.Diagnostics;

namespace newTests
{
    public class UnitTest1
    {
        [Fact]
        public void HeaderProcessor_EmptyHeader()
        {
            var headerProcessor = new HeaderProcessor("Target");

            Assert.Throws<InvalidFileFormatApplicationException>(() => 
                headerProcessor.ProcessTokenHeader(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void HeaderProcessor_NonExistentColumnNameOneColumn()
        {
            var headerProcessor = new HeaderProcessor("Target");

            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget"));

            Assert.Throws<NonExistentColumnNameApplicationException>(() =>
                headerProcessor.ProcessTokenHeader(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void HeaderProcessor_NonExistentColumnNameMoreColumns()
        {
            var headerProcessor = new HeaderProcessor("Target");

            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget1"));
            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget2"));
            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget3"));

            Assert.Throws<NonExistentColumnNameApplicationException>(() =>
                headerProcessor.ProcessTokenHeader(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void HeaderProcessor_FoundColumnNameOneColumn()
        {
            var headerProcessor = new HeaderProcessor("Target");

            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "Target"));

            Assert.Equal(0, headerProcessor.TargetColumnIndex);
        }

        [Fact]
        public void HeaderProcessor_FoundColumnNameMoreColumns()
        {
            var headerProcessor = new HeaderProcessor("Target");

            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget1"));
            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget2"));
            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "NotTarget3"));
            headerProcessor.ProcessTokenHeader(new Token(TokenType.Word, "Target"));

            Assert.Equal(3, headerProcessor.TargetColumnIndex);
        }

        [Fact]
        public void HeaderProcessor_UnexpectedTokenEndOfInput()
        {
            var headerProcessor = new HeaderProcessor("Target");

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                headerProcessor.ProcessTokenHeader(new Token(TokenType.EndOfInput))
                );
        }

        [Fact]
        public void HeaderProcessor_UnexpectedTokenEndOfParagraph()
        {
            var headerProcessor = new HeaderProcessor("Target");

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                headerProcessor.ProcessTokenHeader(new Token(TokenType.EndOfParagraph))
                );
        }

        [Fact]
        public void TableBodyProcessor_EmptyTableBody()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableBodyProcessor_LineHasLessColumnsThanHeader()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 2);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableBodyProcessor_LineHasMoreColumnsThanHeader()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));
            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "2"));

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableBodyProcessor_LineHasCorrectNumberOfColumns()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine));
        }

        [Fact]
        public void TableBodyProcessor_LineHasCorrectNumberOfColumnsAndCorrectValue()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine));

            Assert.Equal(1, tableBodyProcessor.Sum);
        }

        [Fact]
        public void TableBodyProcessor_LineHasCorrectNumberOfColumnsAndIncorrectValue()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            Assert.Throws<InvalidIntegerValueApplicationException>(() =>
                    tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "a"))
                );
        }

        [Fact]
        public void TableBodyProcessor_LineHasIncorrectNumberOfColumnsAndIncorrectValue()
        {
            var tableBodyProcessor = new TableBodyProcessor(1, 2);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));

            Assert.Throws<InvalidIntegerValueApplicationException>(() =>
                    tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "a"))
                );
        }

        [Fact]
        public void TableBodyProcessor_LineHasIncorrectNumberOfColumnsAndCorrectValue()
        {
            var tableBodyProcessor = new TableBodyProcessor(1, 1);

            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "1"));
            tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.Word, "2"));

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableBodyProcessor_UnexpectedTokenEndOfInput()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfInput))
                );
        }

        [Fact]
        public void TableBodyProcessor_UnexpectedTokenEndOfParagraph()
        {
            var tableBodyProcessor = new TableBodyProcessor(0, 1);

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableBodyProcessor.ProcessTokenTableBody(new Token(TokenType.EndOfParagraph))
                );
        }

        [Fact]
        public void TableSummatorProcessor_EmptyTable()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableSummatorProcessor_TableWithHeaderOnly()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "Target"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableSummatorProcessor_TableWithHeaderAndOneLine()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "Target"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "1"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));
            tableSummatorProcessor.Finish();

            Assert.Equal("Target\n------\n1\n", stringWriter.ToString().Replace("\r\n", "\n"));
        }

        [Fact]
        public void TableSummatorProcessor_TableWithHeaderAndOneLineAndIncorrectValue()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "Target"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));

            Assert.Throws<InvalidIntegerValueApplicationException>(() =>
                    tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "a"))
                );
        }

        [Fact]
        public void TableSummatorProcessor_TableWithHeaderAndOneLineAndIncorrectNumberOfColumns()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "Target"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "1"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "2"));

            Assert.Throws<InvalidFileFormatApplicationException>(() =>
                tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine))
                );
        }

        [Fact]
        public void TableSummatorProcessor_TableWithHeaderAndOneLineAndCorrectValue()
        {
            var stringWriter = new StringWriter();
            var tableSummatorProcessor = new TableSummatorProcessor(stringWriter, "Target");

            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "Target"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.Word, "1"));
            tableSummatorProcessor.ProcessToken(new Token(TokenType.EndOfLine));
            tableSummatorProcessor.Finish();

            Assert.Equal("Target\n------\n1\n", stringWriter.ToString().Replace("\r\n", "\n"));
        }
    }
}*/