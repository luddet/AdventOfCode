using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day09
{
	class Program
	{
		static void Main(string[] args)
		{
			var parser = new StreamParser();
			using (var input = File.OpenText("input.txt"))
			{
				parser.Parse(input);
			}

			Console.WriteLine($"Part 1: {parser.TotalScore}");
			Console.WriteLine($"Part 2: {parser.GarbageCount}");
			Console.ReadLine();


		}
	}

	class StreamParser
	{
		public int NumberOfGroups { get; private set; }
		public int TotalScore { get; private set; }
		public int GarbageCount { get; private set; }


		private int m_currentDepth;

		private void Reset()
		{
			m_currentDepth = 0;
			NumberOfGroups = 0;
			TotalScore = 0;
			GarbageCount = 0;
		}

		public void Parse(TextReader reader)
		{
			Reset();
			bool eos = false;
			int ch;
			while (!eos && !(eos = ((ch = reader.Read()) == -1)))
			{
				switch ((char)ch)
				{
					case '{':
						// enter group
						++m_currentDepth;
						++NumberOfGroups;
						TotalScore += m_currentDepth;
						break;
					case '}':
						// exit group
						--m_currentDepth;
						break;
					case '!':
						// skip next and break if at end of stream
						eos = reader.Read() == -1;
						break;
					case '<':
						// enter garbage
						eos = ConsumeGarbage(reader);
						break;
				}

			}

		}

		/// <summary>
		/// Returns true if end of stream was reached
		/// </summary>
		/// <param name="reader"></param>
		/// <returns></returns>
		private bool ConsumeGarbage(TextReader reader)
		{
			bool eos = false;
			int ch;
			while (!eos && !(eos = (ch = reader.Read()) == -1))
			{
				switch ((char) ch)
				{
					case '!':
						// skip next but break if at end of stream
						eos = reader.Read() == -1;
						break;
					case '>':
						return false;
					default:
						++GarbageCount;
						break;

				}
			}
			return true;
		}
	}

	[TestFixture]
	class StreamParserTests
	{
		private static readonly IReadOnlyList<Tuple<string, int>> s_groupsTestInput = new List<Tuple<string, int>>
		{	// input stream, correct number of groups
			Tuple.Create("{}",1),
			Tuple.Create("{{{}}}",3),
			Tuple.Create("{{},{}}",3),
			Tuple.Create("{{{},{},{{}}}}",6),
			Tuple.Create("{<{},{},{{}}>}",1),
			Tuple.Create("{<a>,<a>,<a>,<a>}",1),
			Tuple.Create("{{<a>},{<a>},{<a>},{<a>}}",5),
			Tuple.Create("{{<!>},{<!>},{<!>},{<a>}}",2),
		};

		private static readonly IReadOnlyList<Tuple<string, int>> s_scoreTestInput = new List<Tuple<string, int>>
		{	// input stream, correct score
			Tuple.Create("{}",1),
			Tuple.Create("{{{}}}",6),
			Tuple.Create("{{},{}}",5),
			Tuple.Create("{{{},{},{{}}}}",16),
			Tuple.Create("{<a>,<a>,<a>,<a>}",1),
			Tuple.Create("{{<ab>},{<ab>},{<ab>},{<ab>}}",9),
			Tuple.Create("{{<!!>},{<!!>},{<!!>},{<!!>}}",9),
			Tuple.Create("{{<a!>},{<a!>},{<a!>},{<ab>}}",3),
		};

		private static readonly IReadOnlyList<Tuple<string, int>> s_garbageCountTestInput = new List<Tuple<string, int>>
		{
			Tuple.Create("<>",0),
			Tuple.Create("<random characters>",17),
			Tuple.Create("<<<<>",3),
			Tuple.Create("<{!>}>",2),
			Tuple.Create("<!!>",0),
			Tuple.Create("<!!!>>",0),
			Tuple.Create("<{o\"i!a,<{i<a>",10),
		};

		[Test]
		public void StreamParser_ParseTakesAStream()
		{
			new StreamParser().Parse(new StringReader(string.Empty));
		}

		[TestCaseSource(nameof(s_groupsTestInput))]
		public void StreamParser_Parse_NumberOfGroupsAreCorrect(Tuple<string, int> input)
		{
			var parser = new StreamParser();

			using (var reader = new StringReader(input.Item1))
				parser.Parse(reader);

			Assert.That(parser.NumberOfGroups, Is.EqualTo(input.Item2));
		}

		[TestCaseSource(nameof(s_scoreTestInput))]
		public void StreamParser_Parse_ScoreIsCorrect(Tuple<string, int> input)
		{
			var parser = new StreamParser();

			using (var reader = new StringReader(input.Item1))
				parser.Parse(reader);

			Assert.That(parser.TotalScore, Is.EqualTo(input.Item2));
		}

		[TestCaseSource(nameof(s_garbageCountTestInput))]
		public void StreamParser_Parse_GarbageCountIsCorrect(Tuple<string, int> input)
		{
			var parser = new StreamParser();

			using (var reader = new StringReader(input.Item1))
				parser.Parse(reader);

			Assert.That(parser.GarbageCount, Is.EqualTo(input.Item2));
		}

	}
}
