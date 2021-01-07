using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using NUnit.Framework.Api;
using NUnit.Framework.Constraints;

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

		[Test]
		public void FindCommonLetters()
		{
			var input = new[]
			{
				"abcde",
				"fghij",
				"klmno",
				"pqrst",
				"fguij",
				"axcye",
				"wvxyz"
			};

			var expectedResult = "fgij";

			var result = Day02.FindCommonLetters(input);

			Assert.That(result, Is.EqualTo(expectedResult));
		}

		[TestCase("abcde", "axcye", ExpectedResult = "ace")]
		[TestCase("fghij", "fguij", ExpectedResult = "fgij")]
		public string FindCommon(string str1, string str2)
		{
			return Day02.FindCommonLetters(str1, str2);
		}
	}


	class Day02
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			var checksum = CheckSum(input);
			var commonLetters = FindCommonLetters(input);
			Console.WriteLine($"Day02 part 1 result: {checksum}");
			Console.WriteLine($"Day02 part 2 result: {commonLetters}");
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

		

		public static string FindCommonLetters(string[] input)
		{
			for (int i = 0; i < input.Length - 1; ++i)
			{
				for (int j = i + 1; j < input.Length; ++j)
				{
					var common = FindCommonLetters(input[i], input[j]);
					if (common.Length == input[i].Length - 1)
						return common;
				}
			}
			return string.Empty;
		}

		public static string FindCommonLetters(string str1, string str2)
		{
			var charCount = Math.Min(str1.Length, str2.Length);
			var sb = new StringBuilder(charCount);
			for (var i = 0; i < charCount; ++i)
			{
				if (str1[i] == str2[i])
					sb.Append(str1[i]);
			}
			return sb.ToString();
		}
	}
}