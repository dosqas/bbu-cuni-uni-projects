namespace c_a1a2
{
    internal class FormCharsIntoStrings
    {
        private bool isWord = false;
        private bool newLineDetected = false;

        public string ProcessChar(char c)
        {
            if (c == '\r')
            {
                return "";
            }
            if (c == ' ' || c == '\n' || c == '\t')
            {
                if (isWord)
                {
                    // If we meet a whitespace character and we were in a word, we end the word.
                    if (c == '\n')
                    {
                        // If the whitespace character is a newline, we also set newLineDetected to true.
                        this.newLineDetected = true;
                    }
                    this.isWord = false;
                    return "WORD_ENDED";
                }
                if (c == '\n' && newLineDetected)
                {
                    // If we meet a newline character and we still had no non-newline characters met
                    // since the last newline character, we consider this a new paragraph.
                    return "NEW_PARA";
                }
                if (c == '\n')
                {
                    // If we meet a newline character, we set newLineDetected to true.
                    newLineDetected = true;
                }
            }
            else
            {
                if (c == '\0' && isWord)
                {
                    // If we are at the end of the file and we were in a word, we end the word.
                    return "WORD_ENDED";
                }
                isWord = true;
                newLineDetected = false;
            }
            // The status does not matter if we are not ending a word or a paragraph.
            return "";
        }
    }
}
