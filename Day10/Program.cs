using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Day10
{
	internal class Bot
	{
		private readonly List<int> m_values = new List<int>();

		public Bot(int id)
		{
			Id = id;
		}

		public int Id { get; }

		public void AddValue(int value)
		{
			if  (m_values.Count > 1)
				throw new InvalidOperationException("Added more than two values to bot: " + Id);
			m_values.Add(value);
		}

		public int High
		{
			get
			{
				if (m_values.Count < 2)
					throw new InvalidOperationException("Bot " + Id + " has less than two values.");
				return m_values.Max();
			}
		}
		public int Low {
			get
			{
				if (m_values.Count < 2)
					throw new InvalidOperationException("Bot " + Id + " has less than two values.");
				return m_values.Min();
			}
		}

	}

	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var standardRegex = new Regex(@"((?<source>bot \d+) gives low to (?<low_dest>(bot|output) \d+) and high to (?<high_dest>(bot|output) \d+)|(?<source>value \d+) goes to (?<dest>bot \d+))", RegexOptions.ExplicitCapture | RegexOptions.Compiled);

			var result = standardRegex.Match(input.First());
			foreach (var name in standardRegex.GetGroupNames())
				Console.WriteLine($"{name}: {result.Groups[name]}");
			Console.ReadLine();
		}
	}
}
