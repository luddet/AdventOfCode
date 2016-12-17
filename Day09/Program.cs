using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day09
{
	enum StreamState
	{
		None,
		Marker,
		Data,
	}

	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");

			input = "ADVENT";

			var result = Decode(input);

		}

		public static string Decode(string input)
		{
			return input;
		}
	}

	[TestFixture]
	class ProgramTests
	{
		[Test]
		public void Decode_NoMarkers_ReturnsSameString()
		{
			const string input = "ADVENTOFCODE";
			var result = Program.Decode(input);

			Assert.That(result, Is.EqualTo(input));
		}

		[TestCase("A(1x5)BC", ExpectedResult = "ABBBBBC")]
		[TestCase("(3x3)XYZ", ExpectedResult = "XYZXYZXYZ")]
		[TestCase("A(2x2)BCD(2x2)EFG", ExpectedResult = "ABCBCDEFEFG")]
		[TestCase("(6x1)(1x3)A", ExpectedResult = "(1x3)A")]
		[TestCase("X(8x2)(3x3)ABCY", ExpectedResult = "X(3x3)ABC(3x3)ABCY")]
		public string Decode(string input)
		{
			return Program.Decode(input);
		}
	}
}
