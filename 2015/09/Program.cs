using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace _09
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			var lookup = ParseInput(input);

			var cities = lookup.GetCities().ToList();

			

		}

		internal static ICollection<T> Permutate<T>(ICollection<T> input)
		{
			throw new NotImplementedException();
		} 



		internal static DistanceLookup ParseInput(string[] input)
		{
			var result = new DistanceLookup();

			var pattern = @"(?<city1>\w+)\s+to\s+(?<city2>\w+)\s*=\s*(?<distance>\d+)";
			var regex = new Regex(pattern, RegexOptions.Compiled);

			foreach (var line in input)
			{
				var match = regex.Match(line);
				if (!match.Success)
					throw new ArgumentException("Failed to parse: " + line);

				var city1 = match.Groups["city1"].Value;
				var city2 = match.Groups["city2"].Value;
				var distance = int.Parse(match.Groups["distance"].Value);

				result.Add(city1, city2, distance);				
			}

			return result;
		}
	}

	class DistanceLookup
	{
		private readonly Dictionary<string, Dictionary<string, int>> m_lookup = new Dictionary<string, Dictionary<string, int>>();

		public void Add(string city1, string city2, int distance)
		{
			if (string.IsNullOrEmpty(city1))
				throw new ArgumentException("Argument cannot be null or empty string", nameof(city1));
			if (string.IsNullOrEmpty(city2))
				throw new ArgumentException("Argument cannot be null or empty string", nameof(city2));
			if (city1.Equals(city2))
				throw new ArgumentException("Cities cannot be equal");
			if (distance <= 0)
				throw new ArgumentException("Argument cannot be zero or negative", nameof(distance));

			InternalAdd(city1, city2, distance);
			InternalAdd(city2, city1, distance);
		}

		private void InternalAdd(string city1, string city2, int distance)
		{
			Dictionary<string, int> innerLookup;
			if (!m_lookup.TryGetValue(city1, out innerLookup))
			{
				innerLookup = new Dictionary<string, int>();
				m_lookup.Add(city1, innerLookup);
			}

			innerLookup[city2] = distance;
		}

		public bool TryGetDistance(string city1, string city2, out int? distance)
		{
			distance = null;

			Dictionary<string, int> innerLookup;
			if (!m_lookup.TryGetValue(city1, out innerLookup))
				return false;

			int storedDistance;
			if (!innerLookup.TryGetValue(city2, out storedDistance))
				return false;

			distance = storedDistance;
			return true;
		}

		public IEnumerable<string> GetCities()
		{
			return m_lookup.Keys;
		}

	}

	[TestFixture]
	class DistanceLookupTest
	{
		[Test]
		public void Add_ValidStringsAndDistance_NoException()
		{
			var sut = new DistanceLookup();
			sut.Add("London", "Dublin", 123);
		}

		[TestCase("", "abc")]
		[TestCase("abc", "")]
		[TestCase(null, "asd")]
		[TestCase("asd", null)]
		public void Add_CityIsNullOrEmpty_ThrowsArgumentException(string city1, string city2)
		{
			var sut = new DistanceLookup();
			Assert.Throws<ArgumentException>(() => sut.Add(city1, city2, 1) );
		}

		[Test]
		public void Add_CitiesAreEqual_ThrowsArgumentException()
		{
			var sut = new DistanceLookup();
			Assert.Throws<ArgumentException>(() => sut.Add("a", "a", 1));
		}

		[TestCase(0)]
		[TestCase(-1)]
		public void Add_DistanceIsZeroOrNegative_ThrowsArgumentException(int distance)
		{
			var sut = new DistanceLookup();
			Assert.Throws<ArgumentException>(() => sut.Add("asd", "bfe", distance));
		}

		[Test]
		public void TryGetDistance_EmptyLookup_ReturnsFalseAndNull()
		{
			var sut = new DistanceLookup();
			int? resultDistance;
			bool result = sut.TryGetDistance("a", "b", out resultDistance);

			Assert.That(result, Is.False);
			Assert.That(resultDistance, Is.Null);
		}

		[Test]
		public void TryGetDistance_ExistingCities_ReturnsTrueAndCorrectDistance()
		{
			var sut = new DistanceLookup();
			var city1 = "a";
			var city2 = "b";
			var distance = 123;
			sut.Add(city1, city2, distance);

			int? resultDistance;
			var success = sut.TryGetDistance(city1, city2, out resultDistance);

			Assert.That(success, Is.True);
			Assert.That(resultDistance, Is.Not.Null);
			Assert.That(resultDistance, Is.EqualTo(distance));
		}

		[TestCase("a", "c")]
		[TestCase("c", "b")]
		public void TryGetDistance_NonExistingCities_ReturnsFalseAndNull(string city1, string city2)
		{
			var sut = new DistanceLookup();
			sut.Add("a", "b", 1);

			int? distance;
			bool success = sut.TryGetDistance(city1, city2, out distance);

			Assert.That(success, Is.False);
			Assert.That(distance, Is.Null);
		}

	}

	[TestFixture]
	class TestProgram
	{
		[Test]
		public void ParseInput()
		{
			var input = new[]
			{
				"London to Dublin = 464",
				"London to Belfast = 518",
				"Dublin to Belfast = 141",
			};

			var result = Program.ParseInput(input);

			int? distance;
			bool success = result.TryGetDistance("London", "Dublin", out distance);
			Assert.That(success, Is.True);
			Assert.That(distance, Is.EqualTo(464));

			success = result.TryGetDistance("Dublin", "London", out distance);
			Assert.That(success, Is.True);
			Assert.That(distance, Is.EqualTo(464));

			success = result.TryGetDistance("Belfast", "Dublin", out distance);
			Assert.That(success, Is.True);
			Assert.That(distance, Is.EqualTo(141));
		}
	}
}
