using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

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

			ulong result2 = Decode_LengthOnly(input);

			Console.WriteLine("Part 1 decoded length: {0}", result.Length );
			Console.WriteLine("Part 2 decoded length: {0}", result2 );
			
			Console.ReadLine();

		}

		public static ulong Decode_LengthOnly(string input)
		{
			var str = Regex.Replace(input, @"\s", "");
			return Decode_LengthOnly_Internal(str);
		}

		private static ulong Decode_LengthOnly_Internal(string input)
		{
			ulong decodedLength = 0;
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
							decodedLength += (ulong) blockLength;

						nextBlockType = i == -1 ? BlockType.DecodingDone : BlockType.Marker;
						break;
					}

					case BlockType.Marker:
					{
						var nextMarkerIndex = input.IndexOf(MARKER_END, i);
						Debug.Assert(input[i] == MARKER_BEGIN);
						Debug.Assert(nextMarkerIndex != -1);

						blockLength =  nextMarkerIndex - i + 1;
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
						decodedLength += (ulong)dataBlockRepeat * Decode_LengthOnly(currentBlock);

						blockLength = dataBlockLength;
						nextBlockType = BlockType.None;
						break;
					}

					default:
						throw new UnhandledStateException(nextBlockType);
				}

				i += blockLength;
			}

			return decodedLength;
		}


		public static string Decode(string input, bool recursive = false)
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
							decoded.Append(recursive ? Decode(currentBlock, true) : currentBlock);

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
}
