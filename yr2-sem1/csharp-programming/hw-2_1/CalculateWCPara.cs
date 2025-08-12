namespace cs_a1a2
{
    // We use CalculateWCPara to just return the calculated wordcount of the paragraph(s).
    internal class CalculateWCPara
    {
        private int count = 0;

        public void AddCount()
        {
            // We add to the count when a word ends.
            this.count++;
        }

        public void Calculate()
        {
            // We reset the count when a new paragraph is detected.
            this.count = 0;
        }

        public int getCurrentWC()
        {
            // We return the current word count.
            return this.count;
        }

    }
}
