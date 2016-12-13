using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day01
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");
			int? firstIntersection;
			var distance = CalculateDistance(input, out firstIntersection);

			Console.WriteLine("Distance: " + distance);
			if (firstIntersection != null)
				Console.WriteLine("First intersection: " + firstIntersection.Value);

			Console.ReadLine();

		}

		internal static int CalculateDistance(string input, out int? distanceToFirstIntersection)
		{
			distanceToFirstIntersection = null;
			int direction = 0;
			int hPos = 0;
			int vPos = 0;

			var visitedPositions = new HashSet<Tuple<int, int>> {Tuple.Create(0, 0)};

			foreach (var instruction in input.Split(new[] { ", " }, StringSplitOptions.RemoveEmptyEntries))
			{

				var rotation = instruction[0];
				if (rotation == 'L')
					direction = (direction + 3) % 4;
				else
					direction = (direction + 1) % 4;

				var moveDistance = int.Parse(instruction.Substring(1));
				for (int i = 0; i < moveDistance; ++i)
				{
					bool added = false;
					switch (direction)
					{
						case 0:
							added = visitedPositions.Add(Tuple.Create(hPos, ++vPos));
							break;
						case 1:
							added = visitedPositions.Add(Tuple.Create(++hPos, vPos));
							break;
						case 2:
							added = visitedPositions.Add(Tuple.Create(hPos, --vPos));
							break;
						case 3:
							added = visitedPositions.Add(Tuple.Create(--hPos, vPos));
							break;
					}
					if (!added && distanceToFirstIntersection == null)
						distanceToFirstIntersection = Math.Abs(hPos) + Math.Abs(vPos);
				}
			}

			return Math.Abs(hPos) + Math.Abs(vPos);
		}



	}

	[TestFixture]
	class ProgramTests
	{
		[TestCase("R2, L3", ExpectedResult = 5)]
		[TestCase("R2, R2, R2", ExpectedResult = 2)]
		[TestCase("R5, L5, R5, R3", ExpectedResult = 12)]
		public int CalculateDistance(string input)
		{
			int? unused;
			return Program.CalculateDistance(input, out unused);
		}

		[Test]
		public void CalculateDistance_Intersection()
		{
			var input = "R8, R4, R4, R8";
			int expectedResult = 4;

			int? firstIntersection;
			var distance = Program.CalculateDistance(input, out firstIntersection);

			Assert.That(firstIntersection, Is.Not.Null);
			Assert.That(firstIntersection, Is.EqualTo(expectedResult));
		}
	}

	


}
