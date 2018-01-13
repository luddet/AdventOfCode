using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Day16
{
	class Program
	{
		private static string[] s_testInput = new[]
			{
				"s1",
				"x3/4",
				"pe/b"
			};

		private static char[] s_testBuffer = new[]
		{
			'a',
			'b',
			'c',
			'd',
			'e'
		};

		static void Main(string[] args)
		{
			var buffer = new char[16] {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p'};

			var input = File.ReadAllText("input.txt").Split(new []{','}, StringSplitOptions.RemoveEmptyEntries);

//			buffer = s_testBuffer;
//			input = s_testInput;

			foreach (var instruction in input)
			{
				switch (instruction[0])
				{
					case 's':
						Spin(int.Parse(instruction.Substring(1)), buffer);
						break;
					case 'x':
						var digits = instruction.Substring(1).Split('/');
						var a = int.Parse(digits[0]);
						var b = int.Parse(digits[1]);
						Exchange(a, b, buffer);
						break;
					case 'p':
						Partner(instruction[1], instruction[3], buffer);
						break;
					default:
						throw new ApplicationException("Unknown instruction: " + instruction);
				}

			}

			Console.WriteLine($"Part 1: {string.Join("", buffer)}");
			Console.ReadLine();
		}

		private static void Spin(int steps, char[] buffer)
		{
			var bufferSize = buffer.Length;

			var temp = new char[bufferSize];
			buffer.CopyTo(temp, 0);

			for (int i = 0; i < bufferSize; ++i)
				buffer[(i + steps) % bufferSize] = temp[i];
		}

		private static void Exchange(int a, int b, char[] buffer)
		{
			var tmp = buffer[a];
			buffer[a] = buffer[b];
			buffer[b] = tmp;

		}

		private static void Partner(char a, char b, char[] buffer)
		{
			int aIndex = -1;
			int bIndex = -1;
			for (int i = 0; i < buffer.Length; ++i)
			{
				if (buffer[i] == a)
				{
					aIndex = i;
					if (bIndex != -1)
						break;
				}
				else if (buffer[i] == b)
				{
					bIndex = i;
					if (aIndex != -1)
						break;
				}
			}

			var tmp = buffer[aIndex];
			buffer[aIndex] = buffer[bIndex];
			buffer[bIndex] = tmp;
		}
	}
}
