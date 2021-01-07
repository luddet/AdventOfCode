using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace _07
{
	interface ISignal
	{
		ushort GetOutput(Dictionary<string, ISignal> lookupTable);
		void Reset();
	}

	class ConstantSignal : ISignal
	{
		private readonly ushort m_value;
		public ConstantSignal(ushort value)
		{
			m_value = value;
		}

		public ushort GetOutput(Dictionary<string, ISignal> lookupTable)
		{
			return m_value;
		}

		public void Reset()
		{ }
	}

	class UnaryGate : ISignal
	{
		private readonly string m_op;
		private readonly Func<ushort, ushort> m_process;

		public UnaryGate(string op, Func<ushort, ushort> process)
		{
			m_op = op;
			m_process = process;
		}

		public static UnaryGate CreateNotGate(string a)
		{
			return new UnaryGate(a, op => (ushort)~op);
		}

		public static UnaryGate CreateForwardingGate(string a)
		{
			return new UnaryGate(a, op => op);
		}

		#region Implementation of ISignal

		public ushort GetOutput(Dictionary<string, ISignal> lookupTable)
		{
//			Console.WriteLine("Calculating output for: " + m_op);
			return m_process(lookupTable[m_op].GetOutput(lookupTable));
		}

		public void Reset()
		{ }

		#endregion
	}

	class BinaryGate : ISignal
	{
		private ushort? m_cache;
		private readonly string m_a;
		private readonly string m_b;
		private readonly Func<ushort, ushort, ushort> m_process;

		public BinaryGate(string a, string b, Func<ushort, ushort, ushort> process)
		{
			m_a = a;
			m_b = b;
			m_process = process;
		}

		public static BinaryGate CreateOrGate(string a, string b)
		{
			return new BinaryGate(a, b, (op1, op2) => (ushort) (op1 | op2));
		}

		public static BinaryGate CreateAndGate(string a, string b)
		{
			return new BinaryGate(a, b, (op1, op2) => (ushort) (op1 & op2));
		}

		public static BinaryGate CreateLShiftGate(string a, string b)
		{
			return new BinaryGate(a, b,
				(op1, op2) => (ushort) (op1 << op2));
		}

		public static BinaryGate CreateRShiftGate(string a, string b)
		{
			return new BinaryGate(a, b,
				(op1, op2) => (ushort)(op1 >> op2));
		}

		public ushort GetOutput(Dictionary<string, ISignal> lookupTable)
		{
			if (!m_cache.HasValue)
			{
				ushort a, b;
				var signalA = (ushort.TryParse(m_a, out a)) ? new ConstantSignal(a) : lookupTable[m_a];
				var signalB = (ushort.TryParse(m_b, out b)) ? new ConstantSignal(b) : lookupTable[m_b];

//				Console.WriteLine("Calculating output for: " + m_a);
				var aValue = signalA.GetOutput(lookupTable);

//				Console.WriteLine("Calculating output for: " + m_b);
				var bValue = signalB.GetOutput(lookupTable);
				m_cache = m_process(aValue, bValue);
			}
			return m_cache.Value;
		}

		public void Reset()
		{
			m_cache = null;
		}

	}

	class Program
	{
		static void Main(string[] args)
		{
			string[] testInput = new[]
			{
				"123 -> x",
				"456 -> y",
				"x AND y -> d",
				"x OR y -> e",
				"x LSHIFT 2 -> f",
				"y RSHIFT 2 -> g",
				"NOT x -> h",
				"NOT y -> i"
			};

			var expectedTestResult = new Dictionary<string, ushort>()
			{
				{"d", 72},
				{"e", 507},
				{"f", 492},
				{"g", 114},
				{"h", 65412},
				{"i", 65079},
				{"x", 123},
				{"y", 456},
			};

			var input = File.ReadAllLines("input.txt");

			Dictionary<string, ISignal> signals;
			var parseResult = ParseInput(input, out signals);

			if (parseResult)
			{
				string key = "a";
				var value = signals[key].GetOutput(signals);
				Console.WriteLine(key + " : " + value);

				foreach (var s in signals.Values)
					s.Reset();

				signals["b"] = new ConstantSignal(value);
				value = signals[key].GetOutput(signals);
				Console.WriteLine("new value of a: " + value);


			}
			else
				Console.WriteLine("Failed to parse");
			Console.ReadLine();
		}

		private static bool ParseInput(string[] input, out Dictionary<string, ISignal> signals)
		{
			signals = new Dictionary<string, ISignal>();
			foreach (var line in input)
			{
				ISignal signal;
				string key;
				if (TryParseLine(line, out signal, out key))
				{
					signals.Add(key, signal);
				}
				else
				{
					Console.WriteLine("Failed to parse line: {0}", line);
					return false;
				}
			}
			return true;
		}

		private static bool TryParseLine(string line, out ISignal signal, out string key)
		{
			var keyMatch = Regex.Match(line, @"->\s*(\w+)$");
			var constantSignalMatch = Regex.Match(line, @"^\s*(\d+)\s*->");
			var unaryGateMatch = Regex.Match(line, @"^(?:(?<operation>NOT)\s+)?(?<operand>\w+)\s*->");
			var binaryGateMatch = Regex.Match(line, @"^(?<opA>\w+)\s+(?<op>AND|OR|LSHIFT|RSHIFT)\s+(?<opB>\w+)\s*->");


			key = null;
			signal = null;

			if (!keyMatch.Success)
				return false;

			key =  keyMatch.Groups[1].Value;

			if (constantSignalMatch.Success)
			{
				var value = ushort.Parse(constantSignalMatch.Groups[1].Value);
				signal = new ConstantSignal(value);
				return true;
			}

			if (unaryGateMatch.Success)
			{
				var operation = unaryGateMatch.Groups["operation"].Value;
				var operand = unaryGateMatch.Groups["operand"].Value;
				switch (operation)
				{
					case "NOT":
						signal = UnaryGate.CreateNotGate(operand);
						break;
					case "":
						signal = UnaryGate.CreateForwardingGate(operand);
						break;
					default:
						return false;

				}
				return true;
			}

			if (binaryGateMatch.Success)
			{
				var operandA = binaryGateMatch.Groups["opA"].Value;
				var operation = binaryGateMatch.Groups["op"].Value;
				var operandB = binaryGateMatch.Groups["opB"].Value;

				switch (operation)
				{
					case "AND":
						signal = BinaryGate.CreateAndGate(operandA, operandB);
						break;
					case "OR":
						signal = BinaryGate.CreateOrGate(operandA, operandB);
						break;
					case "LSHIFT":
						signal = BinaryGate.CreateLShiftGate(operandA, operandB);
						break;
					case "RSHIFT":
						signal = BinaryGate.CreateRShiftGate(operandA, operandB);
						break;
					default:
						return false;
				}
				return true;
			}

			return false;
		}
	}
}
