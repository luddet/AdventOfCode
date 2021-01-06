using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day06
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var temp = Transpose(input);

			Console.WriteLine(PartOne(temp));
			Console.WriteLine(PartTwo(temp));
			Console.ReadLine();

		}

		private static string PartTwo(string[] input)
		{
			var result = input.Select(
				line => line.GroupBy(ch => ch, (ch, list) => new { Ch = ch, Count = list.Count() })
					.OrderBy(i => i.Count)
					.Select(i => i.Ch).First()
			).ToArray();
			return new string(result);
		}

		private static string PartOne(string[] input)
		{
			var result = input.Select(
				line => line.GroupBy(ch => ch, (ch, list) => new {Ch = ch, Count = list.Count()})
					.OrderByDescending(i => i.Count)
					.Select(i => i.Ch).First()
			).ToArray();
			return new string(result);
		}


		private static string[] Transpose(string[] input)
		{
			var result = new StringBuilder[input[0].Length];
			for (int i = 0; i < input[0].Length; ++i)
				result[i] = new StringBuilder();

			foreach (var line in input)
				for (int i = 0; i < line.Length; ++i)
					result[i].Append(line[i]);

			return result.Select(sb => sb.ToString()).ToArray();
		}
	}
}
