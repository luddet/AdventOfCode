using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Day04
{
	class Program
	{
		class Room
		{
			private static readonly Regex s_roomParser = new Regex(@"(?<encryptedName>\D+)-(?<id>\d+)\[(?<checksum>\w+)]", RegexOptions.Compiled);

			private string m_description;
			private string m_decryptedName;

			public Room(string description)
			{
				m_description = description;
				var result = s_roomParser.Match(m_description);
				EncryptedName = result.Groups["encryptedName"].Value;
				Id = int.Parse(result.Groups["id"].Value);
				Checksum = result.Groups["checksum"].Value;
			}

			public string EncryptedName { get; }

			public string DecryptedName => m_decryptedName ?? (m_decryptedName = DecryptName());

			public int Id { get; }

			public string Checksum { get; }

			public bool IsValid()
			{
				return CalculateChecksum(EncryptedName).Equals(Checksum);
			}

			private string CalculateChecksum(string encryptedName)
			{
				return new string(encryptedName.Where(ch => ch != '-').GroupBy( ch => ch, (ch, list) => new { Character = ch, Count = list.Count()})
					.OrderByDescending(group => group.Count).ThenBy(group => group.Character).Select(group => group.Character).Take(5).ToArray());
			}

			public string DecryptName()
			{
				StringBuilder decryptedName = new StringBuilder();
				string toDecrypt = EncryptedName;
				var id = Id;
				foreach (var ch in toDecrypt)
				{
					if (ch == '-')
					{
						decryptedName.Append(' ');
						continue;
					}

					var result = (char) ((ch - 'a' + id)%('z' - 'a' + 1) + 'a');
					decryptedName.Append(result);
				}
				return m_decryptedName = decryptedName.ToString();
			}
		}

		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
			var rooms = input.Select(line => new Room(line)).ToList();
			var sum = rooms.Where(room => room.IsValid()).Sum(room => room.Id);


			Console.WriteLine("Sum of valid rooms: {0}", sum);
			foreach (var room in rooms.Where(room => room.DecryptedName.Contains("northpole")))
				Console.WriteLine($"{room.DecryptedName}: {room.Id}");

			Console.ReadLine();
		}
	}
}
