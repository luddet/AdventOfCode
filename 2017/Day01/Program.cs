using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day01
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt").Where(char.IsDigit).Select(ch => int.Parse(ch.ToString())).ToArray();
			Console.WriteLine($"Part 1: {Sum(input, 1)}");
			Console.WriteLine($"Part 2: {Sum(input, input.Length/2)}");
			Console.ReadLine();
		}

		private static int Sum(int[] input, int stride)
		{
			return input.Where((e, i)=> e == input[ (i + stride) % input.Length] ).Sum();
		}
	}
}
