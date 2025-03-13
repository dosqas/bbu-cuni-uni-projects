namespace cs_a1a2
{
    // We use CalculateWC to just return the calculated wordcount of the array of tokens.
    internal class CalculateWC
    {
        private int count = 0;

        public void AddCount(int addCount)
        {
            this.count += addCount;
        }
        public int Calculate()
        {
            // We just simply the count.
            return this.count;
        }
    }
}
