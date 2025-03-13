using System.Numerics;

namespace c_a1a2
{
    internal class CalculateSum
    {
        private int sum = 0;

        public void Add(int number)
        {
            this.sum += number;
        }

        public int getSum()
        {
            return this.sum;
        }
    }
}
