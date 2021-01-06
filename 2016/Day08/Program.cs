using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace Day08
{
	class Program
	{
		

		static void Main(string[] args)
		{
			var display = new Display(50, 6);
			var input = File.ReadAllLines("input.txt");

//			input = new[]
//			{
//				"rect 4x4",
//				"rotate row y=2 by 51",
//				"rotate column x=3 by 7"
//
//			};

			foreach (var line in input)
			{
				var temp = line.Split(new []{' '}, 2);
				var commandString = temp[0];
				var parameters = temp[1];

				switch (commandString)
				{
					case "rect":
						display.Rect(parameters);
						break;
					case "rotate":
						display.Rotate(parameters);
						break;
					default:
						Console.WriteLine("Unhandled command");
						break;
				}

			}

			Console.WriteLine(display.GetDisplayString());
			Console.WriteLine("Lit pixels: {0}", display.GetNumberOfLitPixels());

			Console.ReadLine();
		}

		class Display
		{
			private readonly bool[] m_pixelBuffer;
			private readonly uint m_width;
			private readonly uint m_height;
			private static readonly Regex s_parameterParser = new Regex(@"(?<direction>\w*)\s+(x|y)=(?<index>\d+)\s+by\s+(?<steps>\d+)");

			public Display(uint width, uint height)
			{
				m_pixelBuffer = new bool[width * height];
				m_width = width;
				m_height = height;
			}

			public void Rect(string parameters)
			{
				var p = parameters.Split('x');
				var width = uint.Parse(p[0]);
				var height = uint.Parse(p[1]);

				for (int row = 0; row < height; ++row)
					for (int col = 0; col < width; ++col)
						m_pixelBuffer[row * m_width + col] = true;
			}


			private enum RotateDirection
			{
				Row,
				Column
			}

			public void Rotate(string parameters)
			{
				var result = s_parameterParser.Match(parameters);
				if (!result.Success)
					throw new ArgumentException(parameters, nameof(parameters));

				uint stride;
				bool[] buffer;

				uint startIndex = uint.Parse(result.Groups["index"].Value);
				var steps = int.Parse(result.Groups["steps"].Value);

				if (result.Groups["direction"].Value == "row")
				{
					stride = 1;
					buffer = new bool[m_width];
					startIndex *= m_width;
				}
				else
				{
					stride = m_width;
					buffer = new bool[m_height];
				}

				for (int i = 0; i < buffer.Length; ++i)
				{
					buffer[(i + steps) % buffer.Length] = m_pixelBuffer[startIndex + i*stride];
				}

				for (int i = 0; i < buffer.Length; ++i)
				{
					m_pixelBuffer[startIndex + i*stride] = buffer[i];
				}
			}

			public int GetNumberOfLitPixels()
			{
				return m_pixelBuffer.Count(p => p);
			}

			public string GetDisplayString()
			{
				var sb = new StringBuilder((int)(m_height * (m_width + 1)));
				for (int i = 0; i < m_pixelBuffer.Length; ++i)
				{
					sb.Append(m_pixelBuffer[i] ? "#" : ".");
					if (i % m_width == (m_width - 1))
						sb.Append(Environment.NewLine);
				}
				return sb.ToString();
			}
		}
	}
}
