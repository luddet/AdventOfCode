using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day10
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt").Split(',').Select(Int32.Parse).ToArray();
			var hash = Hash(input);

			Console.WriteLine($"Part 1: {hash}");
			Console.ReadLine();
		}

		public static int Hash(int[] seed, int bufferSize = 256)
		{
			int pos = 0;
			int skip = 0;

			var buffer = new int[bufferSize];
			for (var i = 0; i < bufferSize; ++i)
				buffer[i] = i;


			foreach (var length in seed)
			{
				var lastIndex = pos + length - 1;
				for (int i = 0; i < length/2; ++i)
				{
					var tmp = buffer[(pos + i) % buffer.Length];
					buffer[(pos + i) % buffer.Length] = buffer[(lastIndex - i) % buffer.Length];
					buffer[(lastIndex - i) % buffer.Length] = tmp;
				}

				pos = (pos + length + skip) % buffer.Length;
				++skip;
			}

			return buffer[0] * buffer[1];
		}
	}

	[TestFixture]
	class Tests
	{
		[Test]
		public void Hash_ReturnsCorrectHash()
		{
			var seed = new[] { 3, 4, 1, 5 };
			var hash = Program.Hash(seed,5);
			Assert.That(hash, Is.EqualTo(12));
		}
	}
}