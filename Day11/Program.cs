using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day11
{
	class Program
	{
		
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt")
							.Trim()
							.Split(new[] {','}, StringSplitOptions.RemoveEmptyEntries);

			var distance = CalculateDistance(input);

			Console.WriteLine($"Part 1: {distance}");
			Console.ReadLine();

		}

		public static int CalculateDistance(IEnumerable<string> steps)
		{
			var pos = new HexCoord(0,0,0);
			foreach (var step in steps)
			{
				switch (step)
				{
					case "ne":
						pos += new HexCoord(1, 0, -1);
						break;
					case "n":
						pos += new HexCoord(0, 1, -1);
						break;
					case "nw":
						pos += new HexCoord(-1, 1, 0);
						break;
					case "sw":
						pos += new HexCoord(-1, 0, 1);
						break;
					case "s":
						pos += new HexCoord(0, -1, 1);
						break;
					case "se":
						pos += new HexCoord(1, -1, 0);
						break;
					default:
						throw new ApplicationException("Unhandled direction: " + step);
				}
			}

			return pos.Length;
		}
	}

	


	class HexCoord : Tuple<int, int, int>
	{
		public HexCoord(int x, int y, int z)
			: base(x, y, z)
		{
			if (x + y + z != 0)
				throw new ArgumentException("x + y + z != 0");
		}

		public int X => Item1;
		public int Y => Item2;
		public int Z => Item3;

		public int Length => (Math.Abs(X) + Math.Abs(Y) + Math.Abs(Z)) / 2;

		public static HexCoord operator+ (HexCoord lhs, HexCoord rhs)
		{
			return new HexCoord(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
		}
	}

	[TestFixture]
	class Tests
	{
		private static readonly IReadOnlyList<Tuple<string[], int>> s_distances = new List<Tuple<string[], int>>
		{
			Tuple.Create(new []{"ne","ne","ne"}, 3),
			Tuple.Create(new []{"ne","ne","sw","sw"}, 0),
			Tuple.Create(new []{"ne","ne","s","s"}, 2),
			Tuple.Create(new []{"se","sw","se","sw","sw"}, 3)
		};

		[TestCaseSource(nameof(s_distances))]
		public void CalculateDistance(Tuple<string[], int> input)
		{
			var expectedResult = input.Item2;
			var result = Program.CalculateDistance(input.Item1);

			Assert.That(result, Is.EqualTo(expectedResult), "input: " + string.Join(",", input.Item1));
			
		}
	}
}
