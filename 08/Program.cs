using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Common;
using NUnit.Framework;
using System.Text.RegularExpressions;

namespace _08
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var totalActualCharacters = input.Sum(NumberOfCharacters);
			var totalLiteralCharacters = input.Sum(line => line.Length);

			var result = totalLiteralCharacters - totalActualCharacters;

			Console.WriteLine(result);
			Console.ReadLine();
		}

		private static Regex s_regex = new Regex(@"\\\\|\\\""|\\x[0-9a-f]{2}", RegexOptions.Compiled);

		public static int NumberOfCharacters(string input)
		{
			if (!input.StartsWith("\"") || !input.EndsWith("\""))
				throw new ArgumentException("String is no good", nameof(input));

			input = input.Substring(1, input.Length - 2);

			var matches = s_regex.Matches(input).OfType<Match>();
			var matchLengths = matches.Select(match => match.Length).Sum();
			var matchCount = matches.Count();
			
			return input.Length - matchLengths + matchCount;
		}
	}

	[TestFixture]
	[TestOf(nameof(Program))]
	class ProgramTest
	{
		[TestCase("\"\"", ExpectedResult = 0)]
		[TestCase("\"abc\"", ExpectedResult = 3)]
		[TestCase("\"aaa\\\"aaa\"", ExpectedResult = 7)]
		[TestCase("\"aa\\aa\\\"aa\\x3daa\"", ExpectedResult = 11)]
		[TestCase("\"\\x27\"", ExpectedResult = 1)]
		public int Test_NumberOfCharacters(string input)
		{
			return Program.NumberOfCharacters(input);
		}
	}
}
