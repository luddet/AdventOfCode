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
			int validCount = 0;
			foreach (var line in File.ReadAllLines("input.txt"))
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
			Console.ReadLine();
		}
	}
}
