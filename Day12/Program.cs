using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day12
{
	class Program
	{
		private static readonly string[] s_testInput =
		{
			"0 <-> 2",
			"1 <-> 1",
			"2 <-> 0, 3, 4",
			"3 <-> 2, 4",
			"4 <-> 2, 3, 6",
			"5 <-> 6",
			"6 <-> 4, 5"
		};

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt").Where(line => !string.IsNullOrWhiteSpace(line)).ToArray();
//			input = s_testInput;

			var uf = new UnionFind(input.Length);

			foreach (var line in input)
			{
				var tmp = line.Split(new[] {" <-> "}, 2, StringSplitOptions.RemoveEmptyEntries);
				var currentId = int.Parse(tmp[0]);
				var connectTo = tmp[1].Split(',').Select(int.Parse).ToArray();

				if (connectTo.Length == 1 && connectTo[0] == currentId)
					continue;

				foreach (var t in connectTo)
					uf.Union(currentId, t);
			}

			var numConnectedTo0 = Enumerable.Range(0, input.Length).Count(n => uf.Connected(0, n));

			Console.WriteLine($"Part 1: {numConnectedTo0}");
			Console.WriteLine($"Part 2: {uf.Count()}");
			Console.ReadLine();
		}


	}

	public class UnionFind
	{
		private readonly int[] m_ids;

		public UnionFind(int size, bool initializeAllComponents = true)
		{
			m_ids = new int[size];
			for (int i = 0; i < size; ++i)
				m_ids[i] = initializeAllComponents ? i : -1;
		}

		public void Union(int a, int b)
		{
			if (m_ids[a] != -1 && m_ids[b] != -1 && m_ids[a] == m_ids[b])
				return;

			int seta = m_ids[a] != -1 ? m_ids[a] : a;
			bool bNotInitialized = m_ids[b] == -1;
			int setb = bNotInitialized ? b : m_ids[b];

			if (bNotInitialized)
			{
				m_ids[a] = seta;
				m_ids[b] = seta;
			}
			else
			{
				for (int i = 0; i < m_ids.Length; ++i)
					if (m_ids[i] == setb)
						m_ids[i] = seta;
			}
		}

		public bool Connected(int a, int b)
		{
			return m_ids[a] != -1 && m_ids[b] != -1 && m_ids[a] == m_ids[b];
		}

		public int Count()
		{
			return m_ids.Distinct().Count(id => id != -1);
		}
	}

	[TestFixture]
	class UnionFindTests
	{
		[Test]
		public void Count_SameAsSizeWhenInstantiated()
		{
			var uf = new UnionFind(2);
			Assert.That(uf.Count(), Is.EqualTo(2));
		}

		[Test]
		public void Count_ZeroWhenCreatedWithoutInitializing()
		{
			var uf = new UnionFind(2, false);
			Assert.That(uf.Count(), Is.EqualTo(0));
		}

		[Test]
		public void Count_OneWhenCreatedWithoutInitializingAfterOneUnion()
		{
			var uf = new UnionFind(3, false);
			uf.Union(1,2);
			Assert.That(uf.Count(), Is.EqualTo(1));
		}

		[Test]
		public void Count_ReturnsOneLessAfterOneUnion()
		{
			var uf = new UnionFind(3);
			uf.Union(0, 2);
			Assert.That(uf.Count(), Is.EqualTo(2));
		}

		[Test]
		public void Connected_NewInstance_ReturnsFalse()
		{
			var uf = new UnionFind(2);
			var result = uf.Connected(0, 1);

			Assert.That(result, Is.False);
		}

		[Test]
		public void Connected_PairHasBeenUnioned_ReturnsTrue()
		{
			var uf = new UnionFind(3);
			uf.Union(1, 2);
			var result = uf.Connected(1, 2);
			Assert.That(result, Is.True);
		}

		[Test]
		public void Connected_OtherPairHasBeenUnioned_ReturnsFalse()
		{
			var uf = new UnionFind(3);
			uf.Union(0, 1);
			var result = uf.Connected(1, 2);
			Assert.That(result, Is.False);
		}

		[Test]
		public void Connected_NotInitializedAndNotUnioned_ReturnsFalse()
		{
			var uf = new UnionFind(3, false);
			var result = uf.Connected(1, 2);
			Assert.That(result, Is.False);
		}

		[Test]
		public void Connected_NotInitialized_PairUnioned_ReturnsTrue()
		{
			var uf = new UnionFind(3, false);
			uf.Union(1, 2);
			var result = uf.Connected(1, 2);
			Assert.That(result, Is.True);
		}

	}
}
