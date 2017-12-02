using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day02
{
	class Program
	{
		private static string[] s_testInput =
		{
			"5 1 9 5",
			"7 5 3",
			"2 4 6 8"
		};


		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			var i = ParseInput(input);
			
			Console.WriteLine("Part 1: " + Part1Checksum(i));
			Console.WriteLine("Part 2: " + Part2Checksum(i));
			Console.ReadLine();
		}

		private static int Part2Checksum(int[][] ints)
		{
			return ints.Select(RowChecksum).Sum();
		}

		private static int RowChecksum(int[] row)
		{
			int sum = 0;
			for (var i = 0; i < row.Length - 1; ++i)
			{
				for (var j = i + 1; j < row.Length; ++j)
				{
					if (row[i] % row[j] == 0)
					{
						sum += row[i] / row[j];
						break;
					}

					if (row[j] % row[i] == 0)
					{
						sum += row[j] / row[i];
						break;
					}
				}
			}
			return sum;
		}

		private static int Part1Checksum(int[][] ints)
		{
			return ints.Sum(row => row.Max() - row.Min());
		}

		private static int[][] ParseInput(string[] inputString)
		{
			return inputString.Select(str => str.Trim().Split('\t').Select(int.Parse).ToArray()).ToArray();
		}

		
	}
}
