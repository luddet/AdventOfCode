using System;
using NUnit.Framework;

namespace Day09
{
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
		[TestCase("AB(2x3)C(1x2)DE(3x2)ABC", ExpectedResult = "ABC(C(C(1x2)DEABCABC")]
		public string Decode(string input)
		{
			return Program.Decode(input);
		}

		[TestCase("(3x3)XYZ", ExpectedResult = "XYZXYZXYZ")]
		[TestCase("X(8x2)(3x3)ABCY", ExpectedResult = "XABCABCABCABCABCABCY")]
		public string Decode_Recursive(string input)
		{
			return Program.Decode(input, true);
		}

		[Test]
		public void Decode_Recursive_LotsOfAs()
		{
			var input = "(27x12)(20x12)(13x14)(7x10)(1x12)A";
			var expected = new string('A', 241920);

			var result = Program.Decode(input, true);

			Assert.That(result, Is.EqualTo(expected));
		}

		[Test]
		public void Decode_Recursive_Complicated()
		{
			var input = "(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN";
			var expectedLength = 445;

			var result = Program.Decode(input, true);

			Assert.That(result.Length, Is.EqualTo(expectedLength));
		}


		[TestCase("ABC", ExpectedResult = 3)] // ABC
		[TestCase("ABC ", ExpectedResult = 3)] // ABC must ignore whitespace
		[TestCase("ABC\n", ExpectedResult = 3)] // ABC must ignore whitespace
		[TestCase("A BC\n", ExpectedResult = 3)] // ABC must ignore whitespace
		[TestCase("A(1x5)BC", ExpectedResult = 7)] // XYZXYZXYZ
		[TestCase("A(2x2)BCD(2x2)EFG", ExpectedResult = 11)] // ABCBCDEFEFG
		[TestCase("(3x3)XYZ", ExpectedResult = 9)] // XYZXYZXYZ
		[TestCase("X(8x2)(3x3)ABCY", ExpectedResult = 20)] // XABCABCABCABCABCABCY
		[TestCase("(27x12)(20x12)(13x14)(7x10)(1x12)A", ExpectedResult = 241920)] 
		[TestCase("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN", ExpectedResult = 445)] 
		public ulong Decode_LengthOnly(string input)
		{
			return Program.Decode_LengthOnly(input);
		}

	}
}