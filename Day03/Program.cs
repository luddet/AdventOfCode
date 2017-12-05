using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day03
{
	class Program
	{
		private static Dictionary<Tuple<int, int>, int> s_sums = new Dictionary<Tuple<int, int>, int>();
		private const int INPUT = 347991;

		static void Main(string[] args)
		{
			Console.WriteLine("Part 1: " + Distance(SpiralToGrid(INPUT)));

			s_sums.Add(Tuple.Create(0, 0), 1);
			int val;
			int i = 2;
			while ((val = NeighbourhoodSum(SpiralToGrid(i))) < INPUT)
				++i;
			Console.WriteLine("Part 2: " + val);
			Console.ReadLine();

		}

		private static int NeighbourhoodSum(Tuple<int, int> coords)
		{
			int sum = 0;
			for (int x = coords.Item1 - 1; x <= coords.Item1 + 1; ++x)
			{
				for (int y = coords.Item2 - 1; y <= coords.Item2 + 1; ++y)
				{
					if (x == coords.Item1 && y == coords.Item2)
						continue;
					int neighbourValue;
					if (s_sums.TryGetValue(Tuple.Create(x, y), out neighbourValue))
						sum += neighbourValue;
				}
			}
			s_sums.Add(coords, sum);
			return sum;
		}

		private static int Distance(Tuple<int, int> coords)
		{
			return Math.Abs(coords.Item1) + Math.Abs(coords.Item2);
		}

		private static Tuple<int, int> SpiralToGrid(int index)
		{
			// find circle-index
			int circleIndex = 1;
			int multiplier = 0;

			int maxSpiralIndexInCircle = 1;

			while (index > maxSpiralIndexInCircle)
			{
				multiplier += circleIndex;
				++circleIndex;
				maxSpiralIndexInCircle = 8 * multiplier + 1;
			}
			--circleIndex;

			int x = circleIndex;
			int y = circleIndex;

			int diff = maxSpiralIndexInCircle - index;
			while (diff > 0 && x > -circleIndex)
			{
				--diff;
				--x;
			}

			while (diff > 0 && y > -circleIndex)
			{
				--diff;
				--y;
			}

			while (diff > 0 && x < circleIndex)
			{
				--diff;
				++x;
			}

			while (diff > 0 && y < circleIndex)
			{
				--diff;
				++y;
			}

			return Tuple.Create(x, y);
		}

	}
}
