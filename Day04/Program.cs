using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework.Constraints;

namespace Day04
{
	internal struct LogEvent
	{
		public enum EventType
		{
			NewShift,
			WakeUp,
			FallAsleep,
			UnknownType
		}

		public LogEvent(string rawLog)
		{
			DateTime = DateTime.Parse(rawLog.Substring(1, 16));
			Event = rawLog.Substring(19);
			Type = ParseEventType(Event);
			GuardId = (Type == EventType.NewShift) ? (uint?) uint.Parse(Event.Split(' ')[1].Substring(1)) : null;
		}

		private static EventType ParseEventType(string e)
		{
			if (e.StartsWith("Guard"))
				return EventType.NewShift;
			if (e.Equals("falls asleep"))
				return EventType.FallAsleep;
			if (e.Equals("wakes up"))
				return EventType.WakeUp;

			return EventType.UnknownType;
		}

		public readonly DateTime DateTime;
		public readonly EventType Type;
		public readonly string Event;
		public readonly uint? GuardId;
	}

	internal class GuardShift
	{
		public GuardShift(DateTime dateTime, uint guardId)
		{
			DateTime = dateTime;
			GuardId = guardId;
		}

		public static List<GuardShift> ParseShifts(IEnumerable<LogEvent> logEvents)
		{
			var shifts = new List<GuardShift>();
			GuardShift currentShift = null;
			DateTime? lastSleep = null;
			foreach (var e in logEvents)
			{
				switch (e.Type)
				{
					case LogEvent.EventType.NewShift:
						currentShift = new GuardShift(e.DateTime, e.GuardId.Value);
						lastSleep = null;
						shifts.Add(currentShift);
						break;
					case LogEvent.EventType.FallAsleep:
						lastSleep = e.DateTime;
						break;
					case LogEvent.EventType.WakeUp:
						Debug.Assert(currentShift != null);
						Debug.Assert(lastSleep.HasValue);
						for (var i = lastSleep.Value.Minute; i < e.DateTime.Minute; ++i)
							currentShift.Awake[i] = false;
						lastSleep = null;
						break;
					case LogEvent.EventType.UnknownType:
					default:
						throw new InvalidEnumArgumentException(nameof(e.Type));
				}
			}
			return shifts;
		}

		public static uint FindMostSleptMinute(IEnumerable<GuardShift> shifts)
		{
			var acc = CreateSleepHistogram(shifts);
			return FindMaxIndex(acc);
		}

		public static uint FindMaxIndex(uint[] acc)
		{
			uint maxIndex = 0;
			uint maxValue = uint.MinValue;
			for (uint i = 0; i < 60; ++i)
				if (acc[i] > maxValue)
				{
					maxValue = acc[i];
					maxIndex = i;
				}

			return maxIndex;
		}

		public static uint[] CreateSleepHistogram(IEnumerable<GuardShift> shifts)
		{
			var acc = new uint[60];
			foreach (var shift in shifts)
			{
				for (var i = 0; i < 60; ++i)
					if (!shift.Awake[i])
						++acc[i];
			}
			return acc;
		}

		public readonly DateTime DateTime;
		public readonly uint GuardId;
		public readonly bool[] Awake = Enumerable.Repeat(true, 60).ToArray();

		public uint GetSleepTime()
		{
			return (uint)Awake.Count(i => i == false);
		}

		public static Dictionary<uint, uint> GetTotalSleepTimesPerGuard(IEnumerable<GuardShift> shifts)
		{
			return shifts.GroupBy(shift => shift.GuardId, shift => shift.GetSleepTime())
				.ToDictionary(group => group.Key, group => (uint)group.Sum(i => i));
		}

		public static uint FindSleepiestGuard(Dictionary<uint, uint> sleepPerGuard)
		{
			return sleepPerGuard.Aggregate( (l,r) => l.Value > r.Value ? l : r).Key;
		}
	}

	internal class Program
	{
		private static void Main(string[] args)
		{
			var input = File.ReadAllLines("input.txt")
				.Select(str => new LogEvent(str))
				.OrderBy(e => e.DateTime)
				.ToArray();

			var shifts = GuardShift.ParseShifts(input);

			// Part 1
			var sleepPerGuard = GuardShift.GetTotalSleepTimesPerGuard(shifts);
			uint sleepiestGuard = GuardShift.FindSleepiestGuard(sleepPerGuard);
			var mostSleptMinute = GuardShift.FindMostSleptMinute(shifts.Where(shift => shift.GuardId == sleepiestGuard));
			var result1 = sleepiestGuard * mostSleptMinute;

			// Part 2
			Dictionary<uint, uint[]> guardToHistogramMap = shifts.GroupBy(shift => shift.GuardId)
				.ToDictionary(group => group.Key, group => GuardShift.CreateSleepHistogram(group.AsEnumerable()));
			var guardWithMostSleptMinute = guardToHistogramMap.Select(i => new {GuardId = i.Key, MaxSleptMinute = GuardShift.FindMaxIndex(i.Value), MaxSleptValue = i.Value.Max()})
				.Aggregate( (a,b) => a.MaxSleptValue > b.MaxSleptValue ? a : b);
			var result2 = guardWithMostSleptMinute.GuardId * guardWithMostSleptMinute.MaxSleptMinute;


			Console.WriteLine($"Day04 part 1: {result1}");
			Console.WriteLine($"Day04 part 2: {result2}");
			Console.ReadLine();

		}




	}
}
