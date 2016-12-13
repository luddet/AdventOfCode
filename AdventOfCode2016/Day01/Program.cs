using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day01
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllText("input.txt");
			var distance = CalculateDistance(input);
			Debug.WriteLine(distance);
			Console.WriteLine(distance);
			Console.ReadLine();

		}

		internal static int CalculateDistance(string input)
		{
			int direction = 0;
			int hPos = 0;
			int vPos = 0;


			foreach (var instruction in input.Split(new[] {", "}, StringSplitOptions.RemoveEmptyEntries))
			{
				var rotation = instruction[0];
				if (rotation == 'L')
					direction = (direction + 3)%4;
				else
					direction = (direction + 1)%4;

				var moveDistance = int.Parse(instruction.Substring(1));
				switch (direction)
				{
					case 0:
						vPos += moveDistance;
						break;
					case 1:
						hPos += moveDistance;
						break;
					case 2:
						vPos -= moveDistance;
						break;
					case 3:
						hPos -= moveDistance;
						break;
				}
			}

			return Math.Abs(hPos) + Math.Abs(vPos);
		}
	}

	[TestFixture]
	class ProgramTests
	{
		[TestCase("R2, L3", ExpectedResult = 5)]
		[TestCase("R2, R2, R2", ExpectedResult = 2)]
		[TestCase("R5, L5, R5, R3", ExpectedResult = 12)]
		public int CalculateDistance(string input)
		{
			return Program.CalculateDistance(input);
		}
	}

	


}
