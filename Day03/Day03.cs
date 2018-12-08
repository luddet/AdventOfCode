using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using NUnit.Framework;

namespace Day03
{
	[TestFixture]
	class ClaimTests
	{
		[Test]
		public void Parse()
		{
			var input = "#3 @ 4,5: 6x7";
			var expectedResult = new Claim {Id = 3, X = 4, Y = 5, Width = 6, Height = 7};

			var result = Claim.Parse(input);

			Assert.That(result, Is.EqualTo(expectedResult));
		}
	}


	struct Claim
	{
		public uint Id;
		public uint X;
		public uint Y;
		public uint Width;
		public uint Height;

		public static Claim Parse(string input)
		{
			var parts = input.Split(new []{'#', ' ', '@', ':', ',', 'x'}, StringSplitOptions.RemoveEmptyEntries);
			var i = 0;
			return new Claim()
			{
				Id = uint.Parse(parts[i++]),
				X = uint.Parse(parts[i++]),
				Y = uint.Parse(parts[i++]),
				Width = uint.Parse(parts[i++]),
				Height = uint.Parse(parts[i])
			};
		}
	}

	class Day03
	{
		static void Main(string[] args)
		{
			var claims = File.ReadAllLines("input.txt").Select(Claim.Parse);
			const int SHEET_SIZE = 1000;
			var grid = new byte[SHEET_SIZE, SHEET_SIZE];
			int multiClaimCount = 0;
			foreach (var claim in claims)
			{
				for (var x = claim.X; x < claim.X + claim.Width; ++x)
				{
					for (var y = claim.Y; y < claim.Y + claim.Height; ++y)
					{
						++grid[x, y];
						if (grid[x, y] == 2)
							++multiClaimCount;
					}
				}
			}

			Console.WriteLine($"Day03 part 1: {multiClaimCount}");
			Console.ReadLine();
		}
	}
}