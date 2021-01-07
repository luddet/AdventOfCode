using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace _04
{
	class Program
	{
		static void Main(string[] args)
		{
			var md5 = MD5.Create();

			const string secret = "ckczppom";
			var sb = new StringBuilder();
			var byteList = new List<byte>();

			int? fiveZeroesNumber = null;
			int? sixZeroesNumber = null;
			var number = 0;
			while (fiveZeroesNumber == null || sixZeroesNumber == null)
			{
				sb.Clear();
				byteList.Clear();

				var key = secret + number;
				byteList.AddRange(key.Select(Convert.ToByte));

				var hash = md5.ComputeHash(byteList.ToArray());

				foreach (var b in hash)
					sb.Append(Convert.ToString(b, 16).PadLeft(2,'0'));

				if (sb.ToString().StartsWith("000000") && sixZeroesNumber == null)
					sixZeroesNumber = number;
				else if (sb.ToString().StartsWith("00000") && fiveZeroesNumber == null)
					fiveZeroesNumber = number;

				++number;
				if (number % 10000 == 0)
					Console.WriteLine(number);
			}

			Console.WriteLine("five zeroes: " + fiveZeroesNumber);
			Console.WriteLine("six zeroes: " + sixZeroesNumber);

			Console.ReadLine();
		}
	}
}
