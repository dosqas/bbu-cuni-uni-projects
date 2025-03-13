using System.Collections.Generic;

namespace c_a1a2
{
    // We will use a Dictionary to store the word-frequency pairs.
    internal class WordFreqDictionary
    {
        private SortedDictionary<string, int> wordsAndFreqs = new SortedDictionary<string, int>();

        public void addWords(string[] words)
        {
            // We check if the key is already in, if so, we just increment the frequency. Else, we
            // insert the new word in the dictionary, with frequency 1.
            foreach (var word in words)
            {
                if (this.wordsAndFreqs.ContainsKey(word))
                    this.wordsAndFreqs[word]++;
                else this.wordsAndFreqs[word] = 1;
            }
        }

        public int getFreq(string word)
        {
            return this.wordsAndFreqs[word];
        }

        public SortedDictionary<string, int> getWords()
        {
            return this.wordsAndFreqs;
        }
    }
}
