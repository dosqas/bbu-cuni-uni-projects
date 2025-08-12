using System;
using System.IO;

namespace cs_assignment3
{
    // This class is used to summarize the values in a specific column of a table.
    public class TableSummarizer : ITokenProcessor
    {
        public int _totalSum;
        public string _columnName;
        private int _columnIndex;
        private int _columnCount;
        private ITokenReader _tokenReader;
        private int _currentColumnIndex;

        public TableSummarizer(ITokenReader tokenReader, string columnName)
        {
            _tokenReader = tokenReader;
            _columnName = columnName;

            _totalSum = 0;
            _columnCount = 0;
            _columnIndex = -1;  // Indicates column not yet found
            _currentColumnIndex = 0;
        }

        public void ProcessHeader()
        {
            ErrHandler errHandler = new ErrHandler();
            Token currentToken;
            bool found = false;

            while (true)
            {
                currentToken = _tokenReader.ReadToken();

                // Handle end of file or end of header line
                if (currentToken.Type == Token.TokenType.EOF || currentToken.Type == Token.TokenType.EOL)
                {
                    break;
                }

                // If the line is empty (EOP at the start), it’s an error
                if (currentToken.Type == Token.TokenType.EOP && _columnCount == 0)
                {
                    throw errHandler.HandleError(ErrHandler.ERROR.LINE);
                }

                // Track columns and check for the specified column name
                if (currentToken.Type == Token.TokenType.WORD)
                {
                    _columnCount++;
                    if (currentToken.Word != null && currentToken.Word.Equals(_columnName) && found == false)
                    {
                        _columnIndex = _columnCount;
                        found = true;
                    }
                }
            }

            if (!found || _columnIndex == -1)
            {
                throw errHandler.HandleError(ErrHandler.ERROR.HEADER);
            }
        }

        public void ProcessRestOfLines()
        {
            ErrHandler errHandler = new ErrHandler();
            Token currentToken;
            bool newLine = true;

            while (true)
            {
                currentToken = _tokenReader.ReadToken();

                if (currentToken.Type == Token.TokenType.EOF)
                {
                    break;
                }

                if (currentToken.Type == Token.TokenType.EOL)
                {
                    // Check if the line has the correct number of columns
                    if (_currentColumnIndex != _columnCount)
                    {
                        throw errHandler.HandleError(ErrHandler.ERROR.LINE);
                    }
                    _currentColumnIndex = 0;
                    newLine = true;
                    continue;
                }

                if (currentToken.Type == Token.TokenType.EOP && newLine)
                {
                    throw errHandler.HandleError(ErrHandler.ERROR.LINE);
                }

                if (currentToken.Type == Token.TokenType.WORD)
                {
                    _currentColumnIndex++;
                    newLine = false;

                    // We only parse if in the correct column
                    if (_currentColumnIndex == _columnIndex)
                    {
                        // Only parse if in the correct column and avoid exceptions by using TryParse
                        if (currentToken.Word != null && int.TryParse(currentToken.Word, out int value))
                        {
                            _totalSum += value;
                            _tokenReader.SkipRest();
                            _currentColumnIndex = 0;
                            newLine = true;
                        }
                        else
                        {
                            // Handle invalid value without an exception
                            throw errHandler.HandleError(ErrHandler.ERROR.VALUE);
                        }
                    }
                }
            }

            if (_currentColumnIndex != 0 && _currentColumnIndex != _columnCount)
            {
                throw errHandler.HandleError(ErrHandler.ERROR.LINE);
            }
        }

        public void ProcessAllTokens()
        {
            ProcessHeader();
            ProcessRestOfLines();
        }

        public void WriteReport(StreamWriter outputWriter)
        {
            outputWriter.WriteLine(_columnName);
            outputWriter.WriteLine(new string('-', _columnName.Length));
            outputWriter.WriteLine(_totalSum);
        }
    }
}
