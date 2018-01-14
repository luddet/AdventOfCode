using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day17
{
	class Program
	{
		static void Main(string[] args)
		{
			const int steps = 356;
			const int totalValues = 2017; // 50000000;

			Node zeroNode = new Node(0);
			zeroNode.Next = zeroNode;

			int idAfter2017 = 0;

			var currentNode = zeroNode;
			for (int i = 1; i <= totalValues; ++i)
			{
				for (int j = 0; j < steps; ++j)
					currentNode = currentNode.Next;

				var newNode = new Node(i, currentNode.Next);
				currentNode = currentNode.Next = newNode;

				if (i == 2017)
					idAfter2017 = currentNode.Next.Id;

				if (i % 100000 == 0)
					Console.WriteLine(i);
			}

			int idAfterZero = zeroNode.Next.Id;

			Console.WriteLine($"Part 1: {idAfter2017}");
			//Console.WriteLine($"Part 2: {idAfterZero}");
			Console.ReadLine();
		}
	}

	class Node
	{
		public int Id { get; }
		public Node Next { get; set; }

		public Node(int id, Node next = null)
		{
			Id = id;
			Next = next;
		}
	}
}
