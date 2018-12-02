using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using NUnit.Framework.Constraints;

namespace Day01
{
	[TestFixture]
	class Day01Tests
	{
		[TestCase(new []{ +1, +1, +1 }, ExpectedResult = 3)]
		[TestCase(new[] { +1, +1, -2 }, ExpectedResult = 0)]
		[TestCase(new[] { -1, -2, -3 }, ExpectedResult = -6)]
		public int CalibrateFrequencyDrift(IEnumerable<int> corrections)
		{
			return Day01.CalibrateFrequencyDrift(corrections);
		}

		[Test]
		public void ParseInput_ValidInput_ReturnsCorrectEnumerable()
		{
			var input = new []{"1", "-2", "+3"};
			var expectedResult = new [] {1, -2, 3};

			var result = Day01.ParseInput(input);

			Assert.That(result, Is.EqualTo(expectedResult));
		}

		[Test]
		public void ParseInput_InvalidInput_Throws()
		{
			var invalidInput = new string[] { "abca", "-2", "+3" };

			Assert.That(() => { Day01.ParseInput(invalidInput).ToList(); }, Throws.TypeOf<FormatException>());
		}

		[TestCase(new[] {+1, -1}, ExpectedResult = 0)]
		[TestCase(new []{+3, +3, +4, -2, -4}, ExpectedResult = 10)]
		[TestCase(new []{-6, +3, +8, +5, -6}, ExpectedResult = 5)]
		[TestCase(new[] { +7, +7, -2, -7, -4}, ExpectedResult = 14)]
		public int FindFirstDuplicateResult(int[] input)
		{
			return Day01.FindFirstDuplicateResult(input);
		}
	}


	class Day01
	{
		static void Main(string[] args)
		{
			var input = File.ReadLines("input.txt");
			var corrections = ParseInput(input).ToArray();
			var part1Result = CalibrateFrequencyDrift(corrections);
			var part2Result = FindFirstDuplicateResult(corrections);
			Console.WriteLine($"Day01 part 1 result: {part1Result}");
			Console.WriteLine($"Day01 part 2 result: {part2Result}");
			Console.ReadLine();
		}

		public static int CalibrateFrequencyDrift(IEnumerable<int> corrections)
		{
			return corrections.Sum();
		}

		public static IEnumerable<int> ParseInput(IEnumerable<string> input)
		{
			return input.Select(str => int.Parse(str));
		}

		public static int FindFirstDuplicateResult(int[] input)
		{
			HashSet<int> results = new HashSet<int>();

			int currentResult = 0;
			int index = 0;
			while (results.Add(currentResult))
			{
				currentResult += input[index];
				index = ++index % input.Length;
			}
			return currentResult;
		}
	}
}
