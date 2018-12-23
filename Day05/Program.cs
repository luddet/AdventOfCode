using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day05
{
	[TestFixture]
	class StringExtensionsTest
	{
		[Test]
		public void EatMatches1()
		{
			const string testString = "aA";

			var replaced = testString.EatMatches(out var result);
			Assert.That(replaced, Is.True);
			Assert.That(result, Is.Empty);
		}

		[Test]
		public void EatMatches2()
		{
			const string testString = "abBA";
			const string expResult1 = "aA";
			string expResult2 = string.Empty;

			var replaced = testString.EatMatches(out var result);
			Assert.That(replaced, Is.True);
			Assert.That(result, Is.EqualTo(expResult1));

			replaced = result.EatMatches(out result);
			Assert.That(replaced, Is.True);
			Assert.That(result, Is.EqualTo(expResult2));

			replaced = result.EatMatches(out result);
			Assert.That(replaced, Is.False);
			Assert.That(result, Is.Empty);
		}

		[Test]
		public void EatMatches3()
		{
			const string testString = "abAB";

			const string expResult1 = testString;

			var replaced = testString.EatMatches(out var result);
			Assert.That(replaced, Is.False);
			Assert.That(result, Is.EqualTo(expResult1));
		}

		[Test]
		public void EatMatches4()
		{
			const string testString = "aabAAB";

			const string expResult1 = testString;

			var replaced = testString.EatMatches(out var result);
			Assert.That(replaced, Is.False);
			Assert.That(result, Is.EqualTo(expResult1));
		}


		[Test]
		public void EatMatches5()
		{
			string testString = "dabAcCaCBAcCcaDA" + Environment.NewLine;
			const string result1 =    "dabAaCBAcaDA";
			const string result2 =    "dabCBAcaDA";

			string reduced;
			bool replaced;

			replaced = testString.Trim().EatMatches(out reduced);
			Assert.That(replaced, Is.True);
			Assert.That(reduced, Is.EqualTo(result1));

			replaced = reduced.EatMatches(out reduced);
			Assert.That(replaced, Is.True);
			Assert.That(reduced, Is.EqualTo(result2));

			replaced = reduced.EatMatches(out reduced);
			Assert.That(replaced, Is.False);
			Assert.That(reduced, Is.EqualTo(result2));
			Assert.That(reduced.Length,Is.EqualTo(10));
		}

	}

	static class StringExtentions
	{
		public static bool EatMatches(this string str, out string reduced)
		{
			bool replaced = false;
			var sb = new StringBuilder();

			for (int i = 0; i < str.Length; ++i)
			{
				if (i == str.Length - 1)
				{
					sb.Append(str[i]);
					break;
				}

				var currentChar = str[i];
				var nextChar = str[i + 1];

				if (char.ToLower(currentChar) != char.ToLower(nextChar)
					|| (char.IsLower(currentChar) && char.IsLower(nextChar))
					|| (char.IsUpper(currentChar) && char.IsUpper(nextChar)))
				{
					sb.Append(currentChar);
				}
				else
				{
					++i;
					replaced = true;
				}

			}

			reduced = sb.ToString();
			return replaced;
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt").Trim();

			string part1Result = input;
			while (part1Result.EatMatches(out part1Result)) {}

			Console.WriteLine($"Day05 Part 1: {part1Result.Length}");
			Console.ReadLine();


		}
	}
}
