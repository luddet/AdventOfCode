using System;
using System.CodeDom;
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
			var iterations = 1000000000;

			// Test overrides
			//			iterations = 2;
			//			buffer = s_testBuffer;
			//			input = s_testInput;

			var context = new DanceContext(buffer);

			var firstBufferState = new char[buffer.Length];
			var operations = ParseOperations(input);

			var states = new Dictionary<DanceContext, int>();

			for (int i = 0; i < iterations; ++i)
			{
				foreach (var operation in operations)
					operation.Execute(context);

				if (states != null)
				{
					if (!states.ContainsKey(context))
					{
						states.Add(context.DeepClone(), i);
					}
					else // Found loop
					{
						states.TryGetValue(context, out int loopStart);
						var loopEnd = i;
						var loopPeriod = i - loopStart;
						var loopSkips = (iterations - loopStart) / loopPeriod;
						i = loopSkips * loopPeriod + loopStart;
						states = null;
						Console.WriteLine($"Found loop from {loopStart} to {loopEnd}, length: {loopPeriod}, finishing up from: {i}");

					}
				}

				if (i == 0)
				{
					context.Buffer.CopyTo(firstBufferState, 0);
					RotateBuffer(context.Spin, firstBufferState);
				}
			}

			RotateBuffer(context.Spin, context.Buffer);
			
			Console.WriteLine($"Part 1: {string.Join("", firstBufferState)}");
			Console.WriteLine($"Part 2: {string.Join("", context.Buffer)}");
			Console.ReadLine();
		}

		private static List<IOperation> ParseOperations(string[] input)
		{
			var operations = new List<IOperation>();
			foreach (var instruction in input)
			{
				switch (instruction[0])
				{
					case 's':
						var newSpin = int.Parse(instruction.Substring(1));
						operations.Add(new SpinOperation(newSpin));
						break;
					case 'x':
						var digits = instruction.Substring(1).Split('/');
						var a = int.Parse(digits[0]);
						var b = int.Parse(digits[1]);
						operations.Add(new ExchangeOperation(a, b));
						break;
					case 'p':
						operations.Add(new PartnerOperation(instruction[1], instruction[3]));
						break;
					default:
						throw new ApplicationException("Unknown instruction: " + instruction);
				}
			}
			return operations;
		}

		private static void RotateBuffer(int steps, char[] buffer)
		{
			var bufferSize = buffer.Length;

			var temp = new char[bufferSize];
			buffer.CopyTo(temp, 0);

			for (int i = 0; i < bufferSize; ++i)
				buffer[(i + steps) % bufferSize] = temp[i];
		}
	}

	internal interface IOperation
	{
		void Execute(DanceContext context);
	}

	internal class SpinOperation : IOperation
	{
		private readonly int m_steps;

		public SpinOperation(int steps)
		{
			m_steps = steps;
		}

		public void Execute(DanceContext context)
		{
			context.Spin = (context.Spin + m_steps) % context.Buffer.Length;
		}
	}

	internal class ExchangeOperation : IOperation
	{
		private readonly int m_b;
		private readonly int m_a;

		public ExchangeOperation(int a, int b)
		{
			m_a = a;
			m_b = b;
		}

		public void Execute(DanceContext context)
		{
			var indexA = (m_a - context.Spin + context.Buffer.Length) % context.Buffer.Length;
			var indexB = (m_b - context.Spin + context.Buffer.Length) % context.Buffer.Length;

			var tmp = context.Buffer[indexA];
			context.Buffer[indexA] = context.Buffer[indexB];
			context.Buffer[indexB] = tmp;
		}
	}

	internal class PartnerOperation : IOperation
	{
		private readonly char m_a;
		private readonly char m_b;

		public PartnerOperation(char a, char b)
		{
			m_a = a;
			m_b = b;
		}

		public void Execute(DanceContext context)
		{
			int aIndex = -1;
			int bIndex = -1;
			for (int i = 0; i < context.Buffer.Length; ++i)
			{
				if (context.Buffer[i] == m_a)
				{
					aIndex = i;
					if (bIndex != -1)
						break;
				}
				else if (context.Buffer[i] == m_b)
				{
					bIndex = i;
					if (aIndex != -1)
						break;
				}
			}

			var tmp = context.Buffer[aIndex];
			context.Buffer[aIndex] = context.Buffer[bIndex];
			context.Buffer[bIndex] = tmp;
		}
	}

	internal class DanceContext
	{
		public DanceContext(char[] buffer)
		{
			Buffer = new char[buffer.Length];
			buffer.CopyTo(Buffer, 0);
			Spin = 0;
		}

		public int Spin { get; set; }
		public readonly char[] Buffer;

		public override int GetHashCode()
		{
			int hash = 17;
			hash = hash * 31 + Spin;
			hash = hash * 31 + string.Join("", Buffer).GetHashCode();
			return hash;
		}

		public override bool Equals(object obj)
		{
			if (obj == null)
				return false;

			if (ReferenceEquals(this, obj))
				return true;

			if (GetType() != obj.GetType())
				return false;

			var other = obj as DanceContext;
			return other != null && Spin == other.Spin && Buffer.SequenceEqual(other.Buffer);
		}

		public DanceContext DeepClone()
		{
			return new DanceContext(Buffer) { Spin = this.Spin };
		}
	}


}
