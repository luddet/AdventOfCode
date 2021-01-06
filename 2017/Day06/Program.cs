using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day06
{
	internal class Memory<T>: IEquatable<Memory<T>>
	{
		private readonly T[] m_banks;

		public Memory(Memory<T> other):
			this(other.m_banks)
		{ }

		public Memory(IEnumerable<T> banks)
		{
			m_banks = banks.ToArray();
		}

		public int Length => m_banks.Length;

		public T this[int i]
		{
			get { return m_banks[i]; }
			set { m_banks[i] = value; }
		}

		public override int GetHashCode()
		{
			int hash = 0x2D2816FE;
			for (int i = 0; i < m_banks.Length; ++i)
				hash += hash * 31 + m_banks[i].GetHashCode();
			return hash;

		}

		public bool Equals(Memory<T> other)
		{
			if (ReferenceEquals(null, other)) return false;
			if (ReferenceEquals(this, other)) return true;
			return m_banks.SequenceEqual(other.m_banks);
		}

		public override bool Equals(object obj)
		{
			if (ReferenceEquals(null, obj)) return false;
			if (ReferenceEquals(this, obj)) return true;
			if (obj.GetType() != this.GetType()) return false;
			return Equals((Memory<T>)obj);
		}


		public override string ToString()
		{
			return "{" + string.Join(", ", m_banks) + "}";
		}
	}

	class Program
	{
		private static int[] s_testInput = { 0, 2, 7, 0 };

		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt")
				.Split(new[] {'\t'}, StringSplitOptions.RemoveEmptyEntries)
				.Select(int.Parse)
				.ToArray();

			var previousConfigurations = new Dictionary<Memory<int>, int>();
			var banks = new Memory<int>(input);
			int steps = 0;
			bool foundPrev;
			int prev;
			do
			{
				var max = GetMaxBank(banks);
				Redistribute(max, banks);
				++steps;
				if (! (foundPrev = previousConfigurations.TryGetValue(banks, out prev)))
					previousConfigurations.Add(new Memory<int>(banks), steps);
			}
			while (!foundPrev);
			Console.WriteLine("Part 1 - Steps: " + steps);
			Console.WriteLine("Part 2 - Loopsize: " + (steps - prev));
			Console.WriteLine("Done");
			Console.ReadLine();

		}

		private static void Redistribute(int bankIndex, Memory<int> banks)
		{
			var blocks = banks[bankIndex];
			banks[bankIndex] = 0;
			var blocksPerBank = blocks / banks.Length;
			var remainder = blocks % banks.Length;
			for (int i = 0; i < banks.Length; ++i)
			{
				banks[(i + bankIndex + 1) % banks.Length] += blocksPerBank + (remainder > 0 ? 1 : 0);
				if (remainder > 0)
					--remainder;
			}
		}

		private static int GetMaxBank(Memory<int> memory)
		{
			var maxBank = memory.Length - 1;
			for (int i = maxBank - 1; i >= 0; --i)
				if (memory[i] >= memory[maxBank])
					maxBank = i;
			return maxBank;
		}
	}
}
