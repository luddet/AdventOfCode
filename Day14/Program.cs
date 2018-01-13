using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Day10;
using NUnit.Framework;

namespace Day14
{
	class Program
	{
		private const string INPUT = "amgozmfv";
		private const string TEST_INPUT = "flqrgnkx";

		static void Main(string[] args)
		{
			var hashes = CalculateHashes(128, INPUT);
			var binary = hashes.Select(ConvertHashToBinary).ToArray();
			var usedSquares = CountUsedSquares(binary);
			
			Console.WriteLine($"Part 1: {usedSquares}");

			Console.ReadLine();
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

			var hashes = Program.CalculateHashes(128, TEST_INPUT);
			var binary = hashes.Select(Program.ConvertHashToBinary).ToArray();
			var result = Program.CountUsedSquares(binary);

			Assert.That(result, Is.EqualTo(expectedResult));

		}
	}

	
}
