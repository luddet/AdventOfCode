using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace _05
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var forbiddenStrings = new [] { "ab", "cd", "pq", "xy" };

			var part1Criteria = new List<Func<string, bool>>
			{
				str => str.Count(ch => ("aeiou").Contains(ch)) >= 3,
				str => Regex.Match(str, @"(\w)\1").Success,
				str => !forbiddenStrings.Any(str.Contains)
			};

			var part2Criteria = new List<Func<string, bool>>
			{
				str => Regex.IsMatch(str, @"(..).*\1"),
				str => Regex.IsMatch(str, @"(.).\1")
			};

			var part1NiceCount = input.Count(str => part1Criteria.All(crit => crit(str)));
			var part2NiceCount = input.Count(str => part2Criteria.All(crit => crit(str)));

			Console.WriteLine("Nice lines 1: {0}", part1NiceCount);
			Console.WriteLine("Nice lines 2: {0}", part2NiceCount);
			Console.ReadLine();
		}

	}
}
