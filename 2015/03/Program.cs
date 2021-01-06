using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Mail;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace _03
{
	class CoordEqualityComparer : EqualityComparer<Coord>
	{
		public override bool Equals(Coord c1, Coord c2)
		{
			if (c1 == null && c2 == null)
				return true;
			else if (c1 == null | c2 == null)
				return false;
			else if (c1.X == c2.X && c1.Y == c2.Y)
				return true;
			else
				return false;
		}

		public override int GetHashCode(Coord obj)
		{
			int hCode = obj.X ^ obj.Y;
			return hCode.GetHashCode();
		}
	}

	class Coord
	{
		public int X;
		public int Y;

		public Coord(int x = 0, int y = 0)
		{
			X = x;
			Y = y;
		}

		public Coord(Coord other)
		{
			X = other.X;
			Y = other.Y;
		}

		public override string ToString()
		{
			return string.Format("{{X={0}, Y={1}}}", X, Y);
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");

			var numberOfHouses1 = CalculatePart1(input);
			var numberOfHouses2 = CalculatePart2(input);

			Console.WriteLine("1. Number of houses that get at least one present: {0}", numberOfHouses1);
			Console.WriteLine("2. Number of houses that get at least one present: {0}", numberOfHouses2);
			Console.ReadLine();
		}

		private static int CalculatePart1(string input)
		{
			var currentCoord = new Coord();
			var visitedHouses = new Dictionary<Coord, int>(new CoordEqualityComparer());

			Deliver(currentCoord, visitedHouses);
			Deliver(currentCoord, visitedHouses);

			foreach (var ch in input)
			{
				if (Move(currentCoord, ch))
					Deliver(currentCoord, visitedHouses);
			}
			return visitedHouses.Count;
		}

		private static int CalculatePart2(string input)
		{
			var coords = new[] {new Coord(), new Coord()};
			var visitedHouses = new Dictionary<Coord, int>(new CoordEqualityComparer());

			Deliver(coords[0], visitedHouses);
			Deliver(coords[0], visitedHouses);

			int currentCoord = 0;
			foreach (var ch in input)
			{
				if (Move(coords[currentCoord], ch))
					Deliver(coords[currentCoord], visitedHouses);
				currentCoord = (++currentCoord)%coords.Length;
			}
			return visitedHouses.Count;
		}

		private static bool Move(Coord currentCoord, char ch)
		{
			switch (ch)
			{
				case '^':
					currentCoord.Y++;
					break;
				case 'v':
					currentCoord.Y--;
					break;
				case '<':
					currentCoord.X--;
					break;
				case '>':
					currentCoord.X++;
					break;
				default:
					return false;
			}
			return true;
		}

		private static void Deliver(Coord currentCoord, Dictionary<Coord, int> visitedHouses)
		{
			if (visitedHouses.ContainsKey(currentCoord))
				visitedHouses[currentCoord]++;
			else
				visitedHouses[new Coord(currentCoord)] = 1;
		}
	}
}
