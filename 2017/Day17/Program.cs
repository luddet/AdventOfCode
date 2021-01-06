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
			const int maxValue = 50000000;


//			var (id1, id2) = ExecuteWithLinkedList(steps, 2017);
			var (id1, id2) = ExecuteWithBufferArray(3, 2017);
			
			Console.WriteLine($"{id1}, {id2}");
			Console.ReadLine();
		}

		public static (int idAfter2017, int idAfterZero) ExecuteWithBufferArray(int steps, int maxValue)
		{
			var buffer = new int[maxValue+1];
			int currentPosition = 0;
			for (int i = 1; i <= maxValue; ++i)
			{
				currentPosition = (currentPosition + steps) % i;
				buffer[i] = buffer[currentPosition];
				currentPosition = buffer[currentPosition] = i;

				DebugPrintBuffer(i+1, buffer);
				if (i > 10)
					break;
			}

			return (buffer[2017], buffer[0]);
		}

		private static void DebugPrintBuffer(int count, int[] buffer)
		{
			int index = 0;
			var sb = new StringBuilder(count);
			for (int j = 0; j < count; ++j)
			{
				sb.Append(index);
				index = buffer[index];
			}
			Console.WriteLine(sb.ToString());
		}

		public static (int idAfter2017, int idAfterZero) ExecuteWithLinkedList(int steps, int totalValues)
		{
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

			return (idAfter2017, idAfterZero);
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
