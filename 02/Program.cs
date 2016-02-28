using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _02
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			int totalPaperNeeded = 0;
			int totalRibbonNeeded = 0;

			foreach (var line in input)
			{
				var dims = line.Split('x').Select(int.Parse).ToArray();

				totalPaperNeeded += CalculatePaperNeed(dims[0], dims[1], dims[2]);
				totalRibbonNeeded += CalculateRibbonNeeded(dims[0], dims[1], dims[2]);
			}

			Console.WriteLine("Paper needed: {0} sq feet", totalPaperNeeded);
			Console.WriteLine("Ribbon needed: {0} feet", totalRibbonNeeded);
			Console.ReadLine();

		}

		private static int CalculateRibbonNeeded(int w, int l, int h)
		{
			// circumferences
			var c0 = 2*w + 2*l;
			var c1 = 2*l + 2*h;
			var c2 = 2*h + 2*w;

			var ribbonNeeded = Math.Min(c0, Math.Min(c1, c2));

			// plus volume
			ribbonNeeded += w*l*h;

			return ribbonNeeded;
		}

		private static int CalculatePaperNeed(int w, int l, int h)
		{
			// surface areas
			var s0 = w*l;
			var s1 = l*h;
			var s2 = h*w;

			var surfaceArea = 2*s0 + 2*s1 + 2*s2;
			var extra = Math.Min(s0, Math.Min(s1, s2));

			return surfaceArea + extra;
		}
	}
}
