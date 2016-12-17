using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day09
{
	class UnhandledStateException : ApplicationException
	{
		public UnhandledStateException(BlockType type): base(type.ToString()) { }
	}

	enum BlockType
	{
		None,
		Marker,
		Data,
		DecodingDone,
	}

	class Program
	{
		private const char MARKER_BEGIN = '(';
		private const char MARKER_END = ')';

		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");
			var result = Decode(input);
			result = Regex.Replace(result, @"\s", "");

			Console.WriteLine(input.Substring(0, 50));
			Console.WriteLine(result.Substring(0, 50));
			Console.WriteLine("decoded length: {0}", result.Length );
			Console.ReadLine();

		}

		public static string Decode(string input)
		{
			var decoded = new StringBuilder();
			var nextBlockType = BlockType.None;

			int dataBlockLength = 0;
			int dataBlockRepeat = 0;

			int i = 0;
			while (i < input.Length && nextBlockType != BlockType.DecodingDone)
			{
				int blockLength;
				switch (nextBlockType)
				{
					case BlockType.None:
					{
						var nextMarkerIndex = input.IndexOf(MARKER_BEGIN, i);
						blockLength = nextMarkerIndex != -1 ? nextMarkerIndex - i : input.Length - i;

						if (blockLength > 0)
							decoded.Append(input.Substring(i, blockLength));

						nextBlockType = i == -1 ? BlockType.DecodingDone : BlockType.Marker;
						break;
					}

					case BlockType.Marker:
					{
						var nextMarkerIndex = input.IndexOf(MARKER_END, i);
						Debug.Assert(input[i] == MARKER_BEGIN);
						Debug.Assert(nextMarkerIndex != -1);

						blockLength = nextMarkerIndex - i + 1;
						var currentBlock = input.Substring(i+1, blockLength-2);

						var dataSpec = currentBlock.ToLower().Split('x');
						Debug.Assert(dataSpec.Length == 2);
						dataBlockLength = int.Parse(dataSpec[0]);
						dataBlockRepeat = int.Parse(dataSpec[1]);

						nextBlockType = BlockType.Data;
						break;
					}

					case BlockType.Data:
					{
						var currentBlock = input.Substring(i, dataBlockLength);
						for (int repeat = 0; repeat < dataBlockRepeat; ++repeat)
							decoded.Append(currentBlock);

						blockLength = dataBlockLength;
						nextBlockType = BlockType.None;
						break;
					}

					default:
						throw new UnhandledStateException(nextBlockType);
				}

				i += blockLength;
			}

			return decoded.ToString();
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
		[TestCase("AB(2x3)C(1x2)DE(3x2)ABC", ExpectedResult = "ABC(C(C(1x2)DEABCABC")]
		public string Decode(string input)
		{
			return Program.Decode(input);
		}
	}
}
