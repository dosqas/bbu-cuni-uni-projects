using System;
using System.IO;

using TokenProcessingFramework;

#nullable enable

namespace TableSummatorApp;


public class HeaderProcessor
{
    private readonly string _targetColumnName;
    private bool _foundTargetColumn = false;
    private int _currentColumn = 0;

    public bool ProcessingColumnHeaders = true;
    public int TargetColumnIndex = -1;
    public int HeaderColumnCount = -1;

    public HeaderProcessor(string targetColumnName)
    {
        _targetColumnName = targetColumnName;
    }

    public void ProcessTokenHeader(Token token)
    {
        switch (token.Type)
        {
            case TokenType.Word:
                if (!_foundTargetColumn && StringComparer.CurrentCultureIgnoreCase.Compare(token.Value, _targetColumnName) == 0)
                {
                    TargetColumnIndex = _currentColumn;
                    _foundTargetColumn = true;
                }
                _currentColumn++;
                break;
            case TokenType.EndOfLine:
                if (_currentColumn == 0)
                {
                    throw new InvalidFileFormatApplicationException();
                }
                else if (!_foundTargetColumn)
                {
                    throw new NonExistentColumnNameApplicationException();
                }
                HeaderColumnCount = _currentColumn;
                _currentColumn = 0;
                ProcessingColumnHeaders = false;
                break;
            default:
                throw new InvalidFileFormatApplicationException();
        }
    }
}


public class TableBodyProcessor
{
    private readonly int _targetColumnIndex;
    private readonly int _headerColumnCount;
    private int _currentColumn = 0;

    public long Sum = 0;

    public TableBodyProcessor(int targetColumnIndex, int headerColumnCount)
    {
        _targetColumnIndex = targetColumnIndex;
        _headerColumnCount = headerColumnCount;
    }

    public void ProcessTokenTableBody(Token token)
    {
        switch (token.Type)
        {
            case TokenType.Word:
                if (_currentColumn == _targetColumnIndex)
                {
                    if (int.TryParse(token.Value!, out int value))
                    {
                        Sum += value;
                    }
                    else
                    {
                        throw new InvalidIntegerValueApplicationException();
                    }
                }
                _currentColumn++;
                break;
            case TokenType.EndOfLine:
                if (_currentColumn == 0 || _currentColumn != _headerColumnCount)
                {
                    throw new InvalidFileFormatApplicationException();
                }
                _currentColumn = 0;
                break;
            default:
                throw new InvalidFileFormatApplicationException();
        }
    }
}




public class TableSummatorProcessor : ITokenProcessor
{
    private HeaderProcessor _headerProcessor;
    private TableBodyProcessor? _tableBodyProcessor = null;
    private TextWriter _outputWriter;
    private string _targetColumnName;

    public TableSummatorProcessor(TextWriter outputWriter, string targetColumnName)
    {
        _outputWriter = outputWriter;
        _targetColumnName = targetColumnName;

        _headerProcessor = new HeaderProcessor(targetColumnName);
    }

    public void ProcessToken(Token token)
    {
        if (_headerProcessor.ProcessingColumnHeaders)
        {
            _headerProcessor.ProcessTokenHeader(token);
            if (!_headerProcessor.ProcessingColumnHeaders)
            {
                _tableBodyProcessor = new TableBodyProcessor(_headerProcessor.TargetColumnIndex, _headerProcessor.HeaderColumnCount);
            }
        }
        else
        {
            _tableBodyProcessor!.ProcessTokenTableBody(token);
        }
    }

    public void Finish()
    {
        if (_headerProcessor.ProcessingColumnHeaders)
        {
            throw new InvalidFileFormatApplicationException();
        }
        _outputWriter.WriteLine(_targetColumnName);
        _outputWriter.WriteLine(new string('-', _targetColumnName.Length));
        _outputWriter.WriteLine(_tableBodyProcessor!.Sum);
    }
}
