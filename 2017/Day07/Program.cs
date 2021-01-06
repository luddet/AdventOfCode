using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using NUnit.Framework;
using NUnit.Framework.Internal;

namespace Day07
{
	[TestFixture]
	internal class NodeTests
	{
		private Node m_nodes;

		[SetUp]
		public void Setup()
		{
			m_nodes = new Node("a", 2);
			m_nodes.Nodes.Add(new Node("b", 3));
			m_nodes.Nodes.Add(new Node("c", 4));
		}

		[Test]
		public void Node_WeightOfChildren()
		{
			var weightOfChildren = m_nodes.WeightOfChildren;
			Assert.That(weightOfChildren, Is.EqualTo(7));
		}

		[Test]
		public void Node_TotalWeight()
		{
			var totalWeight = m_nodes.TotalWeight;
			Assert.That(totalWeight, Is.EqualTo(9));
		}

	}

	class Node: IComparable<Node>
	{
		public Node(string id, int weight)
		{
			Id = id;
			Weight = weight;
		}

		public int Weight { get; }
		public string Id { get; }
		public List<Node> Nodes { get; } = new List<Node>();


		public Node FindNode(string id)
		{
			if (Id == id)
				return this;

			foreach (var node in Nodes)
			{
				var target = node.FindNode(id);
				if (target != null)
					return target;
			}

			return null;
		}

		public int TotalWeight => Weight + WeightOfChildren;
		public int WeightOfChildren => Nodes.Sum(n => n.TotalWeight);
		

		public override string ToString()
		{
			return $"Node({Id}, {Weight}): TotalWeight={TotalWeight}";
		}

		private static readonly Regex s_nodeExpr = new Regex(@"^(?<id>\w+)\s+\((?<weight>\d+)\)",
			RegexOptions.Compiled | RegexOptions.IgnorePatternWhitespace | RegexOptions.ExplicitCapture);

		public static Node Parse(string input)
		{
			var parseResult = s_nodeExpr.Match(input);
			if (!parseResult.Success)
				throw new ArgumentException($"Failed to parse: {input}", nameof(input));
			var id = parseResult.Groups["id"].Value;
			var value = int.Parse(parseResult.Groups["weight"].Value);
			return new Node(id, value);
		}

		public int CompareTo(Node other)
		{
			return Weight == other.Weight ? 0 : (Weight < other.Weight) ? -1 : 1;
		}
	}

	class Program
	{
		
		static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt");
//			input = s_testInput;
			var allNodes = input.Select(Node.Parse).ToDictionary(n => n.Id);
			var nodeTree = BuildTree(allNodes, input);
			int correctWeight = FindCorrectWeight(nodeTree);
			Console.WriteLine("Part 1: " + nodeTree.Id);
			Console.WriteLine("Part 2: " + correctWeight);

			Console.ReadLine();
		}

		private static int FindCorrectWeight(Node nodeTree)
		{
			var currentNode = nodeTree;
			Node oddNode = null;
			int expectedTotal = 0;
			while (true)
			{
				var groups = currentNode.Nodes.GroupBy(n => n.TotalWeight).ToList();
				oddNode = groups.First(g => g.Count() == 1).First();
				if (!oddNode.Nodes.Any() || oddNode.Nodes.All(n => n.TotalWeight == oddNode.Nodes.First().TotalWeight))
				{
					expectedTotal = groups.First(g => g.Count() > 1).First().TotalWeight;
					break;
				}
				currentNode = oddNode;
			}
			var correctWeight = expectedTotal - oddNode.WeightOfChildren;
			return correctWeight;
		}

		private static Node BuildTree(Dictionary<string, Node> nodeList, string[] input)
		{
			foreach (var str in input)
			{
				int idx = str.IndexOf("->", StringComparison.Ordinal);
				if (idx == -1)
					continue;

				var id = str.Substring(0, str.IndexOf(" "));
				if (!nodeList.TryGetValue(id, out Node node))
				{
					node = nodeList.Values.Select(n => n.FindNode(id)).FirstOrDefault(n => n != null);
				}

				var childIds = str.Substring(str.IndexOf("->") + 2).Split(',').Select(substr => substr.Trim());
				foreach (var childId in childIds)
				{
					if (!nodeList.TryGetValue(childId, out Node childNode))
					{
						childNode = nodeList.Values.Select(n => n.FindNode(childId)).FirstOrDefault(n => n != null);
					}
					nodeList.Remove(childId);
					node.Nodes.Add(childNode);
				}
			}

			return nodeList.Values.First();
		}


		private static string[] s_testInput =
		{
			"pbga (66)",
			"xhth (57)",
			"ebii (61)",
			"havc (66)",
			"ktlj (57)",
			"fwft (72) -> ktlj, cntj, xhth",
			"qoyq (66)",
			"padx (45) -> pbga, havc, qoyq",
			"tknk (41) -> ugml, padx, fwft",
			"jptl (61)",
			"ugml (68) -> gyxo, ebii, jptl",
			"gyxo (61)",
			"cntj (57)"
		};

	}
}
