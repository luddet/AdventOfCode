using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day04
{
	class Program
	{

		private static string[] s_anagramTestInput =
		{
			"abcde fghij", // valid
			"abcde xyz ecdab", // invalid
			"a ab abc abd abf abj", // valid
			"iiii oiii ooii oooi oooo", // valid
			"oiii ioii iioi iiio" // invalid
		};

		static void Main(string[] args)
		{
			var passPhrases = File.ReadAllLines("input.txt");
			Console.WriteLine("Part 1 - Valid: " + passPhrases.Count(Validate_NoDuplicates));
			Console.WriteLine("Part 2 - Valid: " + passPhrases.Count(Validate_NoAnagrams));
			Console.ReadLine();
		}

		private static bool Validate_NoDuplicates(string phrase)
		{
			var set = new HashSet<string>();
			return phrase.Split(new[] {' '}, StringSplitOptions.RemoveEmptyEntries).All(set.Add);
		}

		private static bool Validate_NoAnagrams(string phrase)
		{
			var words = phrase.Split(new[] {' '}, StringSplitOptions.RemoveEmptyEntries).ToList();
			for (int i = 0; i < words.Count - 1; ++i)
			{
				var word1 = words[i];
				List<char> word1Sorted = null;
				for (int j = i + 1; j < words.Count; ++j)
				{
					var word2 = words[j];

					if (word1.Length != word2.Length)
						continue;

					if (word1Sorted == null)
					{
						word1Sorted = word1.ToList();
						word1Sorted.Sort();
					}

					var word2Sorted = word2.ToList();
					word2Sorted.Sort();

					if (word1Sorted.SequenceEqual(word2Sorted))
						return false;
				}
			}
			return true;
		}
	}
}
