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
			var currentCoord = new Coord();
			var visitedHouses = new Dictionary<Coord, int>(new CoordEqualityComparer());

			Deliver(currentCoord, visitedHouses);
			foreach (var ch in input)
			{
				if (Move(currentCoord, ch))
					Deliver(currentCoord, visitedHouses);
			}

//			foreach (var kvp in visitedHouses)
//			{
//				Console.WriteLine("{0} : {1}",kvp.Key, kvp.Value );
//			}

			Console.WriteLine("Number of houses that get at least one present: {0}", visitedHouses.Count);
			Console.ReadLine();
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
