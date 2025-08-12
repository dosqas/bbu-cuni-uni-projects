using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HuffmanTree
{
    public class TreeBuilder
    {
        public static Node MakeHuffmanTree(PriorityQueue<Node, (decimal weight, int order, byte? symbol)> priorityQueue)
        {
            int order = 1;
            while (priorityQueue.Count > 1)
            {
                Node left = priorityQueue.Dequeue();
                Node right = priorityQueue.Dequeue();

                if (left.Weight == right.Weight)
                {
                    if (right.Symbol != null) 
                    {
                        if (left.Symbol == null)           
                            Swap(ref left, ref right);
                    }
                    else
                    {
                        if (left.Symbol == null && left.Order > right.Order)
                            Swap(ref left, ref right);
                    }
                }

                Node parent = new Node(weight: left.Weight + right.Weight, order: order++)
                {
                    Left = left,
                    Right = right
                };

                priorityQueue.Enqueue(parent, (parent.Weight, parent.Order, null));
            }

            return priorityQueue.Dequeue();
        }

        public static void Swap(ref Node left, ref Node right)
        {
            Node temp = left;
            left = right;
            right = temp;
        }
    }
}
