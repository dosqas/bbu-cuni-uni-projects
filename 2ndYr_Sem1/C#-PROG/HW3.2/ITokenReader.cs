using cs_assignment3;

namespace cs_assignment3
{
    public interface ITokenReader
    {
        public Token ReadToken();

        public void SkipRest();
    }
}
