using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Day14
{
	class Program
	{
		private static List<string> s_hashes = new List<string>(1024);

		static void Main(string[] args)
		{

			string salt = "ihaygndm";
			var md5 = MD5.Create();
			md5.Initialize();

			int found = 0;
			int index = 0;
			for (; ; ++index)
			{
				char ch;
				var hash = GetHash(salt, index, md5);
				if (ContainsCharsInARow(hash, 3, out ch))
				{
					for (int i = 1; i <= 1000; ++i)
					{
						var nextHash = GetHash(salt, index + i, md5);
						if (nextHash.Contains(new string(ch, 5)))
						{
							if (++found == 64)
							{
								Console.WriteLine("{0}: {1}", index, hash);
								Console.WriteLine("Next with 5: {0}: {1}", index + i, nextHash);
								break;
							}
						}
					}
					if (found == 64)
						break;
				}
			}

			Console.WriteLine("index: " + index);
			Console.ReadLine();
		}


		private static string GetHash(string salt, int index, HashAlgorithm algorithm)
		{
			if (index < s_hashes.Count && s_hashes[index] != null)
				return s_hashes[index];

			var hash = GetHexString(salt + index, algorithm);
			s_hashes.Add(hash);
			return hash;
		}

		public static string GetHexString(string input, HashAlgorithm algorithm)
		{
			var hash = algorithm.ComputeHash(Encoding.ASCII.GetBytes(input), 0, input.Length);
			return BitConverter.ToString(hash).Replace("-", "");
		}

		public static bool ContainsCharsInARow(string input, int numberToLookFor, out char chr)
		{
			if (string.IsNullOrEmpty(input))
			{
				chr = '0';
				return false;
			}

			chr = input[0];
			int charsFound = 1;

			for (int i = 1; i < input.Length; ++i)
			{

				if (input[i] == chr)
				{
					++charsFound;
					if (charsFound == numberToLookFor)
						break;
				}
				else
				{
					chr = input[i];
					charsFound = 1;
				}
			}
				
			return charsFound == numberToLookFor;
		}

		
	}

	[TestFixture]
	internal class ProgramTest
	{
		[Test]
		public void GetHexString_abc18_HasThreeEightsInARow()
		{
			var md5 = MD5.Create();
			md5.Initialize();

			string input = "abc18";
			string result = Program.GetHexString(input, md5);
			
			Assert.That(result.Contains("888"), result);
		}

		[Test]
		public void ContainsCharsInARow_3e()
		{
			string input = "abcdeeefgh";
			char chr;
			bool result = Program.ContainsCharsInARow(input, 3, out chr);
			Assert.That(result, Is.True);
			Assert.That(chr, Is.EqualTo('e'));
		}

		[Test]
		public void ContainsCharsInARow_5b()
		{
			string input = "abbbbbcde";
			char chr;
			bool result = Program.ContainsCharsInARow(input, 5, out chr);
			Assert.That(result, Is.True);
			Assert.That(chr, Is.EqualTo('b'));
		}

		[Test]
		public void ContainsCharsInARow_InputDoesNotContainSearch_ReturnsFalse()
		{
			string input = "abcdefgh";
			char chr;
			bool result = Program.ContainsCharsInARow(input, 5, out chr);
			Assert.That(result, Is.False);
		}

		[Test]
		public void ContainsCharsInARow_InputIsEmpty_ReturnsFalse()
		{
			string input = String.Empty;
			char chr;
			bool result = Program.ContainsCharsInARow(input, 5, out chr);
			Assert.That(result, Is.False);
		}

		[Test]
		public void ContainsCharsInARow_InputIsNull_ReturnsFalse()
		{
			string input = null;
			char chr;
			bool result = Program.ContainsCharsInARow(input, 5, out chr);
			Assert.That(result, Is.False);
		}

	}
}
