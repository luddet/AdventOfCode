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

			var criteria = new List<Func<string, bool>>
			{
				str => str.Count(ch => ("aeiou").Contains(ch)) >= 3,
				str => Regex.Match(str, @"(\w)\1").Success,
				str => !forbiddenStrings.Any(str.Contains)
			};

			var niceCount = input.Count(str => criteria.All(crit => crit(str)));

			Console.WriteLine("Nice lines: {0}", niceCount);
			Console.ReadLine();
		}

	}
}
