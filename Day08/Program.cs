using System;
using System.CodeDom;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day08
{
	class Program
	{
		private static string[] s_testInput =
		{
			"b inc 5 if a > 1",
			"a inc 1 if b < 5",
			"c dec -10 if a >= 1",
			"c inc -20 if c == 10"
		};

		private static Dictionary<string, int> s_registers = new Dictionary<string, int>();

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
//			input = s_testInput;

			Execute(input, s_registers);

			var max = s_registers.Values.Max();

			Console.WriteLine($"Part 1: {max}");
			Console.ReadLine();

		}

		private static void Execute(string[] input, Dictionary<string, int> registers)
		{
			foreach (var line in input)
			{
				var tokens = line.Split(' ').ToList();
				var i = 0;
				var reg = tokens[i++];
				var operation = tokens[i++];
				var value = int.Parse(tokens[i++]);
				++i;
				var conditionReg = tokens[i++];
				var conditionOperator = tokens[i++];
				var conditionValue = int.Parse(tokens[i++]);

				// Check condition
				var existingValue = GetRegister(conditionReg);
				if (!CheckCondition(existingValue, conditionOperator, conditionValue))
					continue;

				// Execute operation
				switch (operation)
				{
					case "inc":
						SetRegister(reg, GetRegister(reg) + value);
						break;
					case "dec":
						SetRegister(reg, GetRegister(reg) - value);
						break;
					default:
						throw new ApplicationException($"Unsupported operation: {operation}");
				}
			}
		}

		private static int GetRegister(string register)
		{
			if (s_registers.TryGetValue(register, out int value))
				return value;

			return s_registers[register] = 0;
		}

		private static void SetRegister(string register, int value)
		{
			s_registers[register] = value;
		}

		private static bool CheckCondition(int value1, string op, int value2)
		{
			switch (op)
			{
				case "<":
					return value1 < value2;
				case ">":
					return value1 > value2;
				case "==":
					return value1 == value2;
				case "!=":
					return value1 != value2;
				case "<=":
					return value1 <= value2;
				case ">=":
					return value1 >= value2;
				default:
					throw new ArgumentException($"Unsupported operator: {op}", nameof(op));

			}
			return false;
		}
	}

	
}
