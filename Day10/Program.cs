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
			var input = File.ReadAllText("input.txt").Split(',').Select(int.Parse).ToArray();
			var hash = new Hasher().Hash(input);

			Console.WriteLine($"Part 1: {hash}");
			Console.ReadLine();
		}
	}

	class Hasher
	{
		private int[] m_buffer;

		public Hasher(int bufferSize = 256)
		{
			m_buffer = new int[bufferSize];
			for (var i = 0; i < bufferSize; ++i)
				m_buffer[i] = i;
		}

		public int Hash(int[] seed)
		{
			int pos = 0;
			int skip = 0;

			foreach (var length in seed)
			{
				var lastIndex = pos + length - 1;
				for (int i = 0; i < length/2; ++i)
				{
					var tmp = m_buffer[(pos + i) % m_buffer.Length];
					m_buffer[(pos + i) % m_buffer.Length] = m_buffer[(lastIndex - i) % m_buffer.Length];
					m_buffer[(lastIndex - i) % m_buffer.Length] = tmp;
				}

				pos = (pos + length + skip) % m_buffer.Length;
				++skip;
			}

			return m_buffer[0] * m_buffer[1];
		}
	}

	[TestFixture]
	class HasherTests
	{
		[Test]
		public void Hash_ReturnsCorrectHash()
		{
			var seed = new[] { 3, 4, 1, 5 };
			var hasher = new Hasher(5);
			var hash = hasher.Hash(seed);
			Assert.That(hash, Is.EqualTo(12));
		}
	}
}