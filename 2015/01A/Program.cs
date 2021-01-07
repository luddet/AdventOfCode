using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AdventOfCode_01
{
	class Program
	{
		static void Main(string[] args)
		{
			string input = File.ReadAllText("input.txt");
			int floor = 0;
			int basementPosition = 1;
			bool foundBasement = false;
			foreach (var ch in input)
			{
				if (ch == '(')
					++floor;
				else if (ch == ')')
					--floor;

				if (!foundBasement)
				{
					if (floor < 0)
						foundBasement = true;
					else
						basementPosition++;
				}
			}

			Console.WriteLine("Ended on Floor: {0}", floor);
			if (foundBasement)
				Console.WriteLine("Basement position: {0}", basementPosition);
			else
				Console.WriteLine("Didn't enter basement");

			Console.ReadLine();
		}
	}
}
