using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
	}


	class Day01
	{
		static void Main(string[] args)
		{
			var input = File.ReadLines("input.txt");
			var corrections = ParseInput(input);
			var result = CalibrateFrequencyDrift(corrections);
			Console.WriteLine($"Day01 part 1 result: {result}");
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
	}
}
