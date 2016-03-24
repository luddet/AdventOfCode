using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Common;
using NUnit.Framework;

namespace _08
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");


		}

		public static int NumberOfCharacters(string input)
		{
			if (!input.StartsWith("\"") || !input.EndsWith("\""))
				throw new ArgumentException("String is no good", nameof(input));

			input = input.Substring(1, input.Length - 2);

			return 0;
		}
	}

	[TestFixture]
	[TestOf(nameof(Program))]
	class ProgramTest
	{
		[TestCase("\"\"", ExpectedResult = 0)]
		[TestCase("\"abc\"", ExpectedResult = 3)]
		[TestCase("\"aaa\"aaa\"", ExpectedResult = 7)]
		[TestCase("\"\x27\"", ExpectedResult = 1)]
		public int Test_NumberOfCharacters(string input)
		{
			return Program.NumberOfCharacters(input);
		}
	}
}
