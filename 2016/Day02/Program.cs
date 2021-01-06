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

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			PartOne(input);
			PartTwo(input);

			Console.ReadLine();
		}

		private static void PartTwo(string[] input)
		{
			var keyTable = new char?[,]
			{
				{null, null, '1', null, null},
				{null, '2', '3', '4',    null},
				{'5', '6', '7', '8', '9' },
				{null, 'A', 'B', 'C', null },
				{null, null, 'D', null, null }
			};

			int hPos = 0;
			int vPos = 2;

			var buttonList = new List<char>();
			foreach (var instruction in input)
			{
				foreach (char ch in instruction)
				{
					switch (ch)
					{
						case 'U':
							if (vPos > 0 && keyTable[hPos, vPos-1] != null)
								--vPos;
							break;
						case 'D':
							if (vPos < keyTable.GetLength(0) - 1 && keyTable[hPos, vPos +1] != null)
								++vPos;
							break;
						case 'R':
							if (hPos < keyTable.GetLength(1) - 1 && keyTable[hPos + 1, vPos] != null)
								++hPos;
							break;
						case 'L':
							if (hPos > 0 && keyTable[hPos - 1, vPos] != null)
								--hPos;
							break;
					}
				}
				buttonList.Add(keyTable[vPos, hPos].Value);
			}
			Console.WriteLine(string.Join("", buttonList));
			


		}




		private static void PartOne(string[] input)
		{
			var keyTable = new int[3, 3]
			{
				{1, 2, 3},
				{4, 5, 6},
				{7, 8, 9}
			};

			int hPos = 1;
			int vPos = 1;
			var buttonList = new List<int>();
			foreach (var instruction in input)
			{
				foreach (char ch in instruction)
				{
					switch (ch)
					{
						case 'U':
							if (vPos > 0)
								--vPos;
							break;
						case 'D':
							if (vPos < 2)
								++vPos;
							break;
						case 'R':
							if (hPos < 2)
								++hPos;
							break;
						case 'L':
							if (hPos > 0)
								--hPos;
							break;
					}
				}
				buttonList.Add(keyTable[vPos, hPos]);
			}
			Console.WriteLine(string.Join("", buttonList));
		}
	}
}
