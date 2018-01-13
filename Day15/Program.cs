using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Day15
{
	class Program
	{
		static void Main(string[] args)
		{
			const int factorA = 16807;
			const int factorB = 48271;

			// Test start values
			//			const int startA = 65;
			//			const int startB = 8921;

			// INPUT:
			// Generator A starts with 699
			// Generator B starts with 124
			const int startA = 699;
			const int startB = 124;

			const int numPairsPart1 = 40_000_000;
			const int numPairsPart2 = 5_000_000;

			var generatorA = Generate(startA, factorA);
			var generatorB = Generate(startB, factorB);

			var numberofMatches = generatorA.Zip(generatorB, Lower16BitsMatch)
											.Take(numPairsPart1)
											.Count(result => result);
			
			Console.WriteLine($"Part 1: {numberofMatches}");

			var generatorA2 = Generate(startA, factorA, 4);
			var generatorB2 = Generate(startB, factorB, 8);

			numberofMatches = generatorA2.Zip(generatorB2, Lower16BitsMatch)
										 .Take(numPairsPart2)
										 .Count(result => result);

			Console.WriteLine($"Part 2: {numberofMatches}");

			Console.ReadLine();
		}

		public static IEnumerable<int> Generate(int firstValue, int factor, int? generateOnlyMultiplesOf = null)
		{
			long previousValue = firstValue;
			while (true)
			{
				previousValue *= factor;
				previousValue %= 2147483647;

				if (generateOnlyMultiplesOf != null && previousValue % generateOnlyMultiplesOf != 0)
					continue;

				yield return (int) previousValue;
			}
		}

		public static bool Lower16BitsMatch(int a, int b)
		{
			return (short) a == (short) b;
		}
	}
}
