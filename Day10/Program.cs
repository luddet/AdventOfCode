using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day10
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt").Trim();

			Part1(input);
			Part2(input);

			Console.ReadLine();
		}

		private static void Part1(string inputString)
		{
			var input = inputString.Split(',').Select(int.Parse).ToArray();
			var hash = new Hasher().SingleHash(input);
			Console.WriteLine($"Part 1: {hash}");
		}

		private static void Part2(string inputString)
		{
			var input = inputString.Select(b => (int)b).ToArray();
			var hash = new Hasher().Hash(input);
			Console.WriteLine($"Part 2: {hash}");

		}
	}

	class Hasher
	{
		private static readonly int[] s_suffix = {17, 31, 73, 47, 23};
		private readonly int[] m_buffer;
		private int m_pos;
		private int m_skip;

		public Hasher(int size = 256)
		{
			m_buffer = new int[size];
			for (var i = 0; i < size; ++i)
				m_buffer[i] = i;

			m_pos = 0;
			m_skip = 0;
		}

		public string Hash(int[] seed)
		{
			var newSeed = new int[seed.Length + s_suffix.Length];
			seed.CopyTo(newSeed, 0);
			s_suffix.CopyTo(newSeed, seed.Length);

			for (int i = 0; i < 64; ++i)
				SingleHash(newSeed);

			var denseHash = new int[16];
			for (int i = 0; i < 16; ++i)
			{
				var xor = m_buffer[i * 16];
				for (int j = i * 16 + 1; j < i * 16 + 16; ++j)
					xor ^= m_buffer[j];
				denseHash[i] = xor;
			}

			var sb = new StringBuilder(32);
			foreach (var n in denseHash)
				sb.AppendFormat("{0:X2}", n);

			return sb.ToString().ToLower();
		}

		public int SingleHash(int[] seed)
		{
			foreach (var length in seed)
			{
				var lastIndex = m_pos + length - 1;
				for (int i = 0; i < length/2; ++i)
				{
					var tmp = m_buffer[(m_pos + i) % m_buffer.Length];
					m_buffer[(m_pos + i) % m_buffer.Length] = m_buffer[(lastIndex - i) % m_buffer.Length];
					m_buffer[(lastIndex - i) % m_buffer.Length] = tmp;
				}

				m_pos = (m_pos + length + m_skip) % m_buffer.Length;
				++m_skip;
			}

			return m_buffer[0] * m_buffer[1];
		}
	}

	[TestFixture]
	class HasherTests
	{
		[Test]
		public void SingleHash_ReturnsCorrectHash()
		{
			var seed = new[] { 3, 4, 1, 5 };
			var hash = new Hasher(5).SingleHash(seed);
			Assert.That(hash, Is.EqualTo(12));
		}

		private static IReadOnlyList<Tuple<string, string>> s_hashes = new List<Tuple<string, string>>
		{
			Tuple.Create("", "a2582a3a0e66e6e86e3812dcb672a272"),
			Tuple.Create("AoC 2017", "33efeb34ea91902bb2f59c9920caa6cd"),
			Tuple.Create("1,2,3", "3efbe78a8d82f29979031a4aa0b16a9d"),
			Tuple.Create("1,2,4", "63960835bcdc130f0b66d7ff4f6a5a8e"),
		};

		[TestCaseSource(nameof(s_hashes))]
		public void Hash(Tuple<string, string> hash)
		{
			var expectedResult = hash.Item2;
			var inputString = hash.Item1;
			var inputArray = inputString.Select(c => (int) c).ToArray();
			var result = new Hasher().Hash(inputArray);

			Assert.That(result, Is.EqualTo(expectedResult));
		}
	}
}