using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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

			Console.WriteLine("Input length: {0}", input.Length );
			Console.WriteLine("Floor: {0}", floor);
			Console.WriteLine("Basement position: {0}", basementPosition);
			Console.ReadLine();
		}
	}
}
