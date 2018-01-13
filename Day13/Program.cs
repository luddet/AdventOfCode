using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day13
{
	class Program
	{
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");

			var firewall = CreateFirewall(input);

			int firstSeverity = CalculateSeverity(firewall, out bool _, 0);
			int delay = CalculateDelay(firewall);

			Console.WriteLine($"Part 1: {firstSeverity}");
			Console.WriteLine($"Part 2: {delay}");
			Console.ReadLine();
		}

		public static string GetFirewallSteps(int[] firewall, int delay = 0)
		{
			var sb = new StringBuilder();
			for (int step = 0, pos = -delay; step < firewall.Length + delay; ++step, ++pos)
			{

				sb.Append($"{step:00}: ");
				for (int i = 0; i < firewall.Length; ++i)
				{
					string scannerPos = firewall[i] > 0 ? GetScannerPos(firewall[i], step).ToString() : "-";
					sb.Append(i == pos ? $"({scannerPos})" : $" {scannerPos} ");
				}
				sb.AppendLine();

			}
			return sb.ToString();
		}

		public static int CalculateDelay(int[] firewall)
		{
			int delay = 0;
			while (true)
			{
				CalculateSeverity(firewall, out bool caught, delay);

				if (!caught)
					return delay;

				++delay;
			};
		}

		public static int CalculateSeverity(int[] firewall, out bool wasCaught, int delay = 0)
		{
			wasCaught = false;
			int severity = 0;
			for (int i = 0; i < firewall.Length; ++i)
			{
				if (firewall[i] == 0)
					continue;

				var scannerPos = GetScannerPos(firewall[i], i + delay);

				if (scannerPos == 0)
				{
					severity += i * firewall[i];
					wasCaught = true;
				}
			}
			return severity;
		}

		public static int GetScannerPos(int range, int step)
		{
			if (range == 0)
				throw new ArgumentException("range can't be zero", nameof(range));

			var pos = step % (2 * range - 2);
			return pos < range ? pos : -pos + 2*(range-1);
		}

		public static int[] CreateFirewall(string[] input)
		{
			var scanners = new List<(int index, int range)>();
			var maxIndex = int.MinValue;
			foreach (var line in input)
			{
				if (string.IsNullOrWhiteSpace(line))
					continue;

				var split = line.Split(':');
				var index = int.Parse(split[0].Trim());
				var range = int.Parse(split[1].Trim());
				if (index > maxIndex)
					maxIndex = index;

				scanners.Add((index, range));
			}

			var firewall = new int[maxIndex + 1];

			foreach (var scanner in scanners)
			{
				firewall[scanner.index] = scanner.range;
			}
			return firewall;
		}
	}

	[TestFixture]
	class ProgramTests
	{
		private static readonly string[] s_testInput =
		{
			"0: 3",
			"1: 2",
			"4: 4",
			"6: 4",
			""
		};

		public static int[] s_correctFirewall = {3, 2, 0, 0, 4, 0, 4};

		private static readonly (int range, int steps, int expectedPosition)[] s_scanners =
		{
			(2, 0, 0),
			(2, 2, 0),
			(3, 2, 2),
			(3, 3, 1),
			(3, 4, 0),
			(3, 5, 1),
			(3, 11, 1),
			(4, 0, 0),
			(4, 1, 1),
			(4, 2, 2),
			(4, 3, 3),
			(4, 4, 2),
			(4, 5, 1),
			(4, 6, 0),
			(4, 7, 1),
			(4, 8, 2),
			(4, 9, 3),
			(5, 11, 3),
		};

		[Test]
		public void CreateFirewall()
		{
			var result = Program.CreateFirewall(s_testInput);

			Assert.That(result, Is.EquivalentTo(s_correctFirewall));

		}

		[TestCaseSource(nameof(s_scanners))]
		public void GetPosition((int range, int steps, int expectedResult) input)
		{
			var result = Program.GetScannerPos(input.range, input.steps);
			Assert.That(result, Is.EqualTo(input.expectedResult));
		}


		private static readonly (int delay, int severity)[] s_severities =
		{
			(0, 24),
			(10, 0)
		};

		[TestCaseSource(nameof(s_severities))]
		public void CalculateSeverity( (int delay, int expectedSeverity) input)
		{
			var result = Program.CalculateSeverity(s_correctFirewall, out bool _, input.delay);
			Assert.That(result, Is.EqualTo(input.expectedSeverity));
		}


		[Test]
		public void CalculateDelay()
		{
			var result = Program.CalculateDelay(s_correctFirewall);
			Assert.That(result, Is.EqualTo(10));
		}

	}
}