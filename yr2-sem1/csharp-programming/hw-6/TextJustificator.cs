using System;
using System.Diagnostics.Tracing;
using System.IO;

namespace cs_assignment3
{
    public class TextJustificator : ITokenProcessor
    {
        private ITokenReader _tokenReader;
        private TextWriter _writer;
        private int _maxTextWidth;
        private bool _newParagraph;
        private bool _lastLineWasParagraph;
        private string[] _wordsLine;
        private int _wordCountLine;
        private int _charLine;
        private bool _enableHighlights;

        public TextJustificator(ITokenReader tokenReader, int maxTextWidth, TextWriter writer, bool enableHighlights = false)
        {
            _tokenReader = tokenReader;
            _writer = writer;
            _maxTextWidth = maxTextWidth;
            _newParagraph = false;
            _lastLineWasParagraph = false;
            _wordsLine = new string[10000];
            _wordCountLine = 0;
            _charLine = 0;
            _enableHighlights = enableHighlights;
        }

        public void ProcessAllTokens()
        {
            Token currentToken = _tokenReader.ReadToken();

            while (currentToken.Type != Token.TokenType.EOF)
            {
                if (currentToken.Type == Token.TokenType.WORD)
                {
                    if (currentToken.Word != null)
                    {
                        if (_charLine + _wordCountLine + currentToken.Word.Length > _maxTextWidth)
                        {
                            if (_wordCountLine > 0)
                            {
                                WriteReport();

                                _wordsLine = new string[1000];
                                _charLine = 0;
                                _wordCountLine = 0;
                            }
                        }
                        _wordsLine[_wordCountLine] = currentToken.Word;
                        _charLine += currentToken.Word.Length;
                        _wordCountLine++;
                    }
                }

                if (currentToken.Type == Token.TokenType.EOP)
                {
                    if (_wordCountLine > 0)
                    {
                        _newParagraph = true;
                        WriteReport();

                        _wordsLine = new string[1000];
                        _charLine = 0;
                        _wordCountLine = 0;
                    }
                }

                currentToken = _tokenReader.ReadToken();
            }

            if (_wordCountLine > 0)
            {
                _newParagraph = true;
                WriteReport();
            }
        }

        public void ProcessAllTokensMultiFile()
        {
            Token currentToken = _tokenReader.ReadToken();

            while (currentToken.Type != Token.TokenType.EOF)
            {
                if (currentToken.Type == Token.TokenType.WORD)
                {
                    if (currentToken.Word != null)
                    {
                        if (_charLine + _wordCountLine + currentToken.Word.Length > _maxTextWidth)
                        {
                            if (_wordCountLine > 0)
                            {
                                WriteReport();

                                _wordsLine = new string[1000];
                                _charLine = 0;
                                _wordCountLine = 0;
                            }
                        }
                        _wordsLine[_wordCountLine] = currentToken.Word;
                        _charLine += currentToken.Word.Length;
                        _wordCountLine++;
                    }
                }

                if (currentToken.Type == Token.TokenType.EOP)
                {
                    if (_wordCountLine > 0)
                    {
                        _newParagraph = true;
                        WriteReport();

                        _wordsLine = new string[1000];
                        _charLine = 0;
                        _wordCountLine = 0;
                    }
                }

                currentToken = _tokenReader.ReadToken();
            }
        }

        public void AllFilesProcessedRoutineMultiFile()
        {
            if (_wordCountLine > 0)
            {
                _newParagraph = true;
                WriteReport();
            }
        }

        public void WriteReport()
        {
            if (_lastLineWasParagraph)
            {
                if (_enableHighlights) _writer.Write("<-");
                _writer.Write("\n");
                _lastLineWasParagraph = false;
            }

            if (_newParagraph)
            {
                for (int i = 0; i < _wordCountLine - 1; i++)
                {
                    _writer.Write(_wordsLine[i]);
                    if (_enableHighlights)
                    {
                        _writer.Write('.');
                    }
                    else _writer.Write(' ');
                }
                _writer.Write(_wordsLine[_wordCountLine - 1]);
                if (_enableHighlights) _writer.Write("<-");
                _writer.Write("\n");

                _newParagraph = false;
                _lastLineWasParagraph = true;
            }
            else
            {
                if (_wordCountLine == 1)
                {
                    _writer.Write(_wordsLine[0]);
                    if (_enableHighlights) _writer.Write("<-");
                    _writer.Write("\n");
                    return;
                }

                int totalSpaces = _maxTextWidth - _charLine;
                int baseSpaces = totalSpaces / (_wordCountLine - 1);
                int extraSpaces = totalSpaces % (_wordCountLine - 1);

                for (int i = 0; i < _wordCountLine - 1; i++)
                {
                    _writer.Write(_wordsLine[i]);
                    if (_enableHighlights)
                    {
                        _writer.Write(new string('.', baseSpaces + (i < extraSpaces ? 1 : 0)));

                    }
                    else _writer.Write(new string(' ', baseSpaces + (i < extraSpaces ? 1 : 0)));
                }
                _writer.Write(_wordsLine[_wordCountLine - 1]);
                if (_enableHighlights) _writer.Write("<-");
                _writer.Write("\n");
            }

        }

        public void SetReader(ITokenReader tokenReader)
        {
            _tokenReader = tokenReader;
        }
    }
}
