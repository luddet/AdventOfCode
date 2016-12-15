using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Day05
{
	class Program
	{
		static void Main(string[] args)
		{
			PartTwo();
			Console.ReadLine();
		}

		private static void PartTwo()
		{
			var hasher = MD5.Create();
			hasher.Initialize();

			var doorId = "ugkcyxxp";

			var password = new char?[8];

			int hashIndex = 0;
			while (password.Any(c => c == null))
			{
				if (hashIndex % 100000 == 0)
					Console.WriteLine("Computing hash {0}", hashIndex);

				var hashInput = Encoding.ASCII.GetBytes(doorId + hashIndex);
				var hexString = BitConverter.ToString(hasher.ComputeHash(hashInput)).Replace("-", "").ToLower();
				if (hexString.StartsWith("00000"))
				{
					int i;
					if (int.TryParse(hexString[5].ToString(), out i) && i < password.Length && password[i] == null)
					{
						password[i] = hexString[6];
						Console.WriteLine("partial password: {0}", new string(password.Select(c => c ?? ' ').ToArray()));
					}
				}
				++hashIndex;
			}

			Console.WriteLine(new string(password.Select(c => c ?? ' ').ToArray()));
		}


		private static void PartOne()
		{
			var hasher = MD5.Create();
			hasher.Initialize();

			var doorId = "ugkcyxxp";

			var password = new char[8];

			for (int i = 0, hashIndex = 0; i < 8; ++hashIndex)
			{
				if (hashIndex%100000 == 0)
					Console.WriteLine("Computing hash {0}", hashIndex);
				var hashInput = Encoding.ASCII.GetBytes(doorId + hashIndex);
				var hexString = BitConverter.ToString(hasher.ComputeHash(hashInput)).Replace("-", "").ToLower();
				if (hexString.StartsWith("00000"))
				{
					password[i] = hexString[5];
					Console.WriteLine("partial password: {0}", new string(password));
					++i;
				}
			}

			Console.WriteLine(new string(password));
		}
	}
}
