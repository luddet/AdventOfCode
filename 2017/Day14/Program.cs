using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Day10;
using Day12;
using NUnit.Framework;

namespace Day14
{
	class Program
	{
		private const string INPUT = "amgozmfv";

		static void Main(string[] args)
		{
			var hashes = CalculateHashes(128, INPUT);
			var binary = hashes.Select(ConvertHashToBinary).ToArray();
			var usedSquares = CountUsedSquares(binary);
			var regions = CountRegions(binary);

			Console.WriteLine($"Part 1: {usedSquares}");
			Console.WriteLine($"Part 2: {regions}");

			Console.ReadLine();
		}

		public static int CountRegions(string[] binary)
		{
			var rowSize = binary[0].Length;
			var uf = new UnionFind(binary.Length * binary[0].Length, false);
			for (int row = 0; row < binary.Length; ++row)
			{
				for (int col = 0; col < rowSize; ++col)
				{
					var index = row * rowSize + col;
					if (binary[row][col] == '0')
						continue;

					// connect to self to create a region regardless if it has neighbours
					uf.Union(index, index);

					// connect top
					if (row > 0 && binary[row - 1][col] == '1')
						uf.Union(index - rowSize, index);
					// connect left
					if (col > 0 && binary[row][col - 1] == '1')
						uf.Union(index - 1, index);
				}
			}
			return uf.Count();
		}

		public static int CountUsedSquares(string[] binary)
		{
			return binary.SelectMany(ch => ch).Count(ch => ch == '1');
		}

		public static string[] CalculateHashes(int size, string input)
		{
			var hasher = new Hasher();
			var hashes = new string[size];
			for (int i = 0; i < hashes.Length; ++i)
			{
				var hashInput = $"{input}-{i}".Select(b => (int) b).ToArray();
				hashes[i] = hasher.Hash(hashInput);
				hasher.Reset();
			}
			return hashes;
		}

		public static string ConvertHashToBinary(string hash)
		{
			var sb = new StringBuilder();
			foreach (var ch in hash)
			{
				sb.Append(Convert.ToString(int.Parse(ch.ToString(), NumberStyles.HexNumber), 2).PadLeft(4, '0'));
			}
			return sb.ToString();
		}
	}

	[TestFixture]
	class ProgramTests
	{
		public const string TEST_INPUT = "flqrgnkx";
		private string[] m_hashes;
		private string[] m_binary;

		[SetUp]
		public void SetUp()
		{
			m_hashes = Program.CalculateHashes(128, TEST_INPUT);
			m_binary = m_hashes.Select(Program.ConvertHashToBinary).ToArray();
		}

		[Test]
		public void ConvertHashToBinary()
		{
			var input = "a0c2017";
			var expectedResult = "1010000011000010000000010111";

			var result = Program.ConvertHashToBinary(input);

			Assert.That(result, Is.EqualTo(expectedResult));
		}

		[Test]
		public void CountUsedSquares()
		{
			var expectedResult = 8108;

			var result = Program.CountUsedSquares(m_binary);

			Assert.That(result, Is.EqualTo(expectedResult));

		}

		[Test]
		public void CountRegions_WithTestInput()
		{
			var expectedResult = 1242;

			var result = Program.CountRegions(m_binary);

			Assert.That(result, Is.EqualTo(expectedResult));

		}

		[Test]
		public void CountRegions()
		{
			string[] input =
			{
				"10010100",
				"11010010",
				"01000001",
				"10111000",
				"10011000",
			};

			var expectedResult = 7;

			var result = Program.CountRegions(input);

			Assert.That(result, Is.EqualTo(expectedResult));
		}
	}

	
}
