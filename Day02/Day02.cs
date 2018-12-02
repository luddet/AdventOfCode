using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using NUnit.Framework.Api;

namespace Day02
{
	[TestFixture]
	class Day02Test
	{
		private static readonly string[] s_input =
			{
				"abcdef",
				"bababc",
				"abbcde",
				"abcccd",
				"aabcdd",
				"abcdee",
				"ababab"
			};

		[Test]
		public void Count_Doubles()
		{

			int twos = Day02.Count(s_input, 2);
			Assert.That(twos, Is.EqualTo(4));
		}

		[Test]
		public void Count_Triples()
		{
			int triples = Day02.Count(s_input, 3);
			Assert.That(triples, Is.EqualTo(3));
		}

		[Test]
		public void CheckSum()
		{
			int expected = 12;
			int result = Day02.CheckSum(s_input);
			Assert.That(result, Is.EqualTo(expected));
		}

	}


	class Day02
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			var checksum = CheckSum(input);
			Console.WriteLine($"Day02 part 1 result: {checksum}");
			Console.ReadLine();
		}

		public static int CheckSum(IEnumerable<string> input)
		{
			var strings = input.ToArray();
			return Count(strings, 2) * Count(strings, 3);
		}

		public static int Count(IEnumerable<string> input, uint count)
		{
			return input.Count(str => str.GroupBy(ch => ch).Any(group => group.Count() == count));
		}
	}
}
