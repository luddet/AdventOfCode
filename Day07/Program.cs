using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Day07
{
	class Program
	{
		// matchar ABBA 
		private static readonly Regex s_abbaMatcher = new Regex(@"(\w)(?!\1)(\w)\2\1", RegexOptions.Compiled);

		// matchar ABBA som har [] runt sig
		private static readonly Regex s_bracketedAbbaMatcher = new Regex(@"(?<=\[\w*)(\w)(?!\1)(\w)\2\1(?=\w*\])", RegexOptions.Compiled);
		private static readonly Regex s_sslMatcher = new Regex(@"\[\w*(\w)(?!\1)(\w)\1\w*\].*?\2\1\2|(\w)(?!\3)(\w)\3.*?\[[^]]*\4\3\4.*\]", RegexOptions.Compiled);

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var count = input.Count(line => s_abbaMatcher.IsMatch(line) && !s_bracketedAbbaMatcher.IsMatch(line));
			int sslCount = 0;
			foreach (var line in input.Where(line => s_sslMatcher.IsMatch(line)))
				++sslCount;

			Console.WriteLine(count);
			Console.WriteLine("sslcount: {0}", sslCount);
			Console.ReadLine();
		}
	}
}
