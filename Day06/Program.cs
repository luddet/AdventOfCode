using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day06
{
	class Point
	{
		public int X;
		public int Y;

		public int Distance(int otherX, int otherY)
		{
			return Math.Abs(otherX - X) + Math.Abs(otherY - Y);
		}

		public override string ToString()
		{
			return $"({X}, {Y})";
		}
	}

	class Program
	{
		static void Main(string[] args)
		{
			var points = File.ReadAllLines("input.txt").Select(line =>
			{
				var c = line.Split(new[] {','});
				return new Point {X = int.Parse(c[0]), Y = int.Parse(c[1])};
			}).ToArray();

			int minX = int.MaxValue;
			int maxX = int.MinValue;
			int minY = int.MaxValue;
			int maxY = int.MinValue;

			foreach (var c in points)
			{
				if (c.X < minX)
					minX = c.X;
				if (c.X > maxX)
					maxX = c.X;
				if (c.Y < minY)
					minY = c.Y;
				if (c.Y > maxY)
					maxY = c.Y;
			}

			var distances = new ushort[(maxX+1) * (maxY+1)];
			var pointIndices = new ushort[(maxX+1) * (maxY+1)];

			for (int x = minX; x <= maxX; ++x)
			{
				for (int y = minY; y < maxY; ++y)
				{
					int shortestDist = int.MaxValue;
					int shortestIndex = 0;
					for (int i = 0; i < points.Length; ++i)
					{
						var currentPoint = points[i];
						var currentDistance = currentPoint.Distance(x, y);
						if (currentDistance < shortestDist)
						{
							distances[x]
						}
					}
				}
			}


			Console.ReadLine();
		}
	}
}
