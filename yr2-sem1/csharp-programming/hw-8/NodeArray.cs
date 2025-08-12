#nullable enable

using System;
using System.Collections.Generic;
using System.Linq;

namespace HuffmanTree
{
    public class NodeArray
    {
        public Node[] Arr;

        public NodeArray()
        {
            Arr = new Node[256];
        }

        public void Add(int value)
        {
            if (Arr[value] == null)
            {
                Arr[value] = new Node(symbol: (byte)value, order:0);
            }

            Arr[value].Weight++;
        }

        public PriorityQueue<Node, (decimal weight, int order, byte? symbol)> GetPriorityQueueFromNodes()
        {
            PriorityQueue<Node, (decimal weight, int order, byte? symbol)> priorityQueue = new PriorityQueue<Node, (decimal weight, int order, byte? symbol)>();

            foreach (Node node in Arr)
            {
                if (node != null) priorityQueue.Enqueue(node, (node.Weight, 0, node.Symbol));
            }

            return priorityQueue;
        }
    }

    public class Node
    {
        public byte? Symbol { get; set; }
        public decimal Weight { get; set; }

        public Node? Left { get; set; }
        public Node? Right { get; set; }
        public int Order;

        public Node(decimal weight, int order)
        {
            this.Symbol = null;
            this.Weight = weight;
            this.Order = order;
        }

        public Node(byte symbol, int order)
        {
            this.Symbol = symbol;
            this.Weight = 0;
            this.Order = 0;
        }
    }
}
