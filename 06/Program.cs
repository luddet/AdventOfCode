using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Http.Headers;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;


namespace _06
{
	enum Action
	{
		TurnOn,
		TurnOff,
		Toggle
	}

	class Instruction
	{
		public Action Action { get; }
		public Rectangle Rectangle { get; }

		public Instruction(Action action, Rectangle rect)
		{
			Action = action;
			Rectangle = rect;
		}
	}

	class Program
	{
		static void Main(string[] args)
		{

			var input = File.ReadAllLines("input.txt");
			var instructions = ParseInput(input);

			var numberOfTurnedOnLights = Part1(instructions);
			uint brightness = Part2(instructions);

			Console.WriteLine("Number of lights on: {0}", numberOfTurnedOnLights);
			Console.WriteLine("Brightness: {0}", brightness);
			Console.ReadLine();
		}

		private static uint Part2(List<Instruction> instructions)
		{
			var grid = new uint[1000, 1000];

			ExecuteInstructionsForPart2(instructions, grid);

			uint brightness = 0;
			foreach (var cell in grid)
			{
				brightness += cell;
			}
			return brightness;

		}

		private static void ExecuteInstructionsForPart2(List<Instruction> instructions, uint[,] grid)
		{
			foreach (var instruction in instructions)
			{
				for (int y = instruction.Rectangle.Top; y <= instruction.Rectangle.Bottom; ++y)
				{
					for (int x = instruction.Rectangle.Left; x <= instruction.Rectangle.Right; ++x)
					{
						switch (instruction.Action)
						{
							case Action.Toggle:
								grid[x, y] += 2; 
								break;
							case Action.TurnOff:
								if (grid[x, y] > 0)
									grid[x, y] -= 1;
								break;
							case Action.TurnOn:
								grid[x, y] += 1;
								break;
						}
					}
				}
			}
		}


		private static int Part1(List<Instruction> instructions)
		{
			var grid = new bool[1000, 1000];

			ExecuteInstructionsForPart1(instructions, grid);

			int numberOfTurnedOnLights = 0;
			foreach (var cell in grid)
			{
				if (cell)
					++numberOfTurnedOnLights;
			}
			return numberOfTurnedOnLights;
		}

		private static void ExecuteInstructionsForPart1(List<Instruction> instructions, bool[,] grid)
		{
			foreach (var instruction in instructions)
			{
				for (int y = instruction.Rectangle.Top; y <= instruction.Rectangle.Bottom; ++y)
				{
					for (int x = instruction.Rectangle.Left; x <= instruction.Rectangle.Right; ++x)
					{
						switch (instruction.Action)
						{
							case Action.Toggle:
								grid[x, y] = !grid[x, y];
								break;
							case Action.TurnOff:
								grid[x, y] = false;
								break;
							case Action.TurnOn:
								grid[x, y] = true;
								break;
						}
					}
				}
			}
		}

		private static List<Instruction> ParseInput(IReadOnlyCollection<string> input)
		{
			var instructions = new List<Instruction>(input.Count);

			foreach (var str in input)
			{
				var action= GetAction(str);
				var rect = GetRect(str);
				instructions.Add(new Instruction(action, rect));
			}

			return instructions;
		}

		private static Rectangle GetRect(string str)
		{
			var result = Regex.Matches(str, @"(\d{1,3}),(\d{1,3})");

			if (result.Count != 2 || !result[0].Success || !result[1].Success)
				throw new Exception("Failed to parse rectangle: " + str);

			int topLeftX, topLeftY, bottomRightX, bottomRightY;

			if (!int.TryParse(result[0].Groups[1].Value, out topLeftX)
			    || !int.TryParse(result[0].Groups[2].Value, out topLeftY)
			    || !int.TryParse(result[1].Groups[1].Value, out bottomRightX)
			    || !int.TryParse(result[1].Groups[2].Value, out bottomRightY))
			{
				throw new Exception("Failed to parse rectangle: " + str);
			}

			return new Rectangle(topLeftX, topLeftY, bottomRightX-topLeftX, bottomRightY-topLeftY);
		}

		private static Action GetAction(string str)
		{
			if (str.StartsWith("toggle"))
				return Action.Toggle;

			if (str.StartsWith("turn on"))
				return Action.TurnOn;

			if (str.StartsWith("turn off"))
				return Action.TurnOff;

			throw new Exception("Unknown instruction: " + str);
		}
	}
}
