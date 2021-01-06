using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day05
{
	class Program
	{
		private static int[] s_testInput = { 0, 3, 0, 1, -3 };

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt").Where(str => !string.IsNullOrEmpty(str)).Select(int.Parse).ToArray();
			Console.WriteLine("Part 1: " + Execute_Part1(input));
			Console.WriteLine("Part 2: " + Execute_Part2(input));
			Console.ReadLine();
		}

		private static int Execute_Part1(int[] instructions)
		{
			instructions = instructions.ToArray();
			int ip = 0, ic = 0;
			while (ip >= 0 && ip < instructions.Length)
			{
				ip += instructions[ip]++;
				++ic;
			}
			return ic;
		}

		private static int Execute_Part2(int[] instructions)
		{
			instructions = instructions.ToArray();
			int ip = 0, ic = 0;
			while (ip >= 0 && ip < instructions.Length)
			{
				ip += instructions[ip] >= 3 ? instructions[ip]-- : instructions[ip]++;
				++ic;
			}
			return ic;
		}
	}
}
