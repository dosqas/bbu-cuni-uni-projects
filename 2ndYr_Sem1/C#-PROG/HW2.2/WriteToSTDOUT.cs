using System;
using System.Collections.Generic;

namespace cs_a1a2
{
    // This class serves to only print to the standard output.
    internal class WriteToSTDOUT
    {
        public void WriteOutput(string output)
        {
            // We use Console.WriteLine to print the output, then a newline.
            // In this example it is very simple but in the future if we need to update it,
            // all we need to do is change how to output.
            Console.WriteLine(output);
        }

        public void WriteOutput(int number)
        {
            // We need to treat int outputs differently.
            Console.WriteLine(number.ToString());
        }

        public void WriteOutput(SortedDictionary<string, int> words)
        {
            // We also treat the dictionary of words-frequencies differently
            foreach (var wordFreqPair in words)
            {
                Console.WriteLine(wordFreqPair.Key + ": " + wordFreqPair.Value);
            }
        }
    }
}
