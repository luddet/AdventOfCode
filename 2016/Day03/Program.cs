using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace Day03
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");
			PartOne(input.Split('\n'));
			PartTwo(input);
			Console.ReadLine();
		}

		private static void PartTwo(string input)
		{
			int validCount = 0;
			var sides = input.Split(new[] {' ', '\n'}, StringSplitOptions.RemoveEmptyEntries).Select(int.Parse).ToList();
			int triangleStride = 9;
			int sideStride = 3;
			for (int offset = 0; offset < 3; ++offset)
			{
				for (int i = offset; i < sides.Count; i+=triangleStride)
				{
					int longest = int.MinValue;
					int sum = 0;
					for (int sideIndex = i; sideIndex < i + triangleStride; sideIndex += sideStride)
					{
						sum += sides[sideIndex];
						if (longest < sides[sideIndex])
							longest = sides[sideIndex];
					}
					if (sum - longest > longest)
						++validCount;
				}
			}
			Console.WriteLine("Valid: {0}", validCount);
		}

		private static void PartOne(string[] input)
		{
			int validCount = 0;
			foreach (var line in input)
			{
				int longest = int.MinValue;
				int sum = 0;
				foreach (var sideString in line.Split(new char[] {' '}, StringSplitOptions.RemoveEmptyEntries))
				{
					var sideLength = int.Parse(sideString);
					sum += sideLength;
					if (longest < sideLength)
						longest = sideLength;
				}
				if (sum - longest > longest)
					++validCount;
			}
			Console.WriteLine("Valid triangles: {0}", validCount);
		}
	}
}
