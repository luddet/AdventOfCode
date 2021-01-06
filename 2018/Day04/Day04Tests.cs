using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;

namespace Day04
{
	[TestFixture]
	class LogEventTests
	{
		[Test]
		public void LogEvent_NewShiftString()
		{
			var shiftStr = "[1518-11-01 23:58] Guard #99 begins shift";
			var result = new LogEvent(shiftStr);

			Assert.That(result.DateTime, Is.EqualTo(new DateTime(1518,11,1,23,58,0)));
			Assert.That(result.GuardId, Is.EqualTo(99));
			Assert.That(result.Event, Is.EqualTo("Guard #99 begins shift"));
			Assert.That(result.Type, Is.EqualTo(LogEvent.EventType.NewShift));
		}

		[Test]
		public void LogEvent_WakeUpString()
		{
			var shiftStr = "[1518-11-01 23:58] wakes up";
			var result = new LogEvent(shiftStr);

			Assert.That(result.DateTime, Is.EqualTo(new DateTime(1518,11,1,23,58,0)));
			Assert.That(result.GuardId, Is.Null);
			Assert.That(result.Event, Is.EqualTo("wakes up"));
			Assert.That(result.Type, Is.EqualTo(LogEvent.EventType.WakeUp));
		}

		[Test]
		public void LogEvent_FallAsleepString()
		{
			var shiftStr = "[1518-11-01 23:58] falls asleep";
			var result = new LogEvent(shiftStr);

			Assert.That(result.DateTime, Is.EqualTo(new DateTime(1518,11,1,23,58,0)));
			Assert.That(result.GuardId, Is.Null);
			Assert.That(result.Event, Is.EqualTo("falls asleep"));
			Assert.That(result.Type, Is.EqualTo(LogEvent.EventType.FallAsleep));
		}
	}

	[TestFixture]
	class GuardShiftTests
	{
		private static readonly string[] s_rawLog =
{
			"[1518-11-01 00:00] Guard #10 begins shift",
			"[1518-11-01 00:05] falls asleep",
			"[1518-11-01 00:25] wakes up",
			"[1518-11-01 00:30] falls asleep",
			"[1518-11-01 00:55] wakes up",
			"[1518-11-01 23:58] Guard #99 begins shift",
			"[1518-11-02 00:40] falls asleep",
			"[1518-11-02 00:50] wakes up",
			"[1518-11-03 00:05] Guard #10 begins shift",
			"[1518-11-03 00:24] falls asleep",
			"[1518-11-03 00:29] wakes up",
			"[1518-11-04 00:02] Guard #99 begins shift",
			"[1518-11-04 00:36] falls asleep",
			"[1518-11-04 00:46] wakes up",
			"[1518-11-05 00:03] Guard #99 begins shift",
			"[1518-11-05 00:45] falls asleep",
			"[1518-11-05 00:55] wakes up",
		};

		private static readonly IEnumerable<LogEvent> s_events = s_rawLog.Select(str => new LogEvent(str)).OrderBy(e => e.DateTime);
		private static readonly List<GuardShift> s_shifts = GuardShift.ParseShifts(s_events);

		[Test]
		public void ParseShifts()
		{

			var shifts = GuardShift.ParseShifts(s_events);

			Assert.That(shifts.Count, Is.EqualTo(5));
			Assert.That(shifts[0].GuardId, Is.EqualTo(10));
			Assert.That(shifts[1].GuardId, Is.EqualTo(99));
			Assert.That(shifts[2].GuardId, Is.EqualTo(10));
			Assert.That(shifts[3].GuardId, Is.EqualTo(99));
			Assert.That(shifts[4].GuardId, Is.EqualTo(99));

			Assert.That(shifts[0].Awake[0], Is.True);
			Assert.That(shifts[0].Awake[6], Is.False);
			Assert.That(shifts[4].Awake[10], Is.True);
			Assert.That(shifts[4].Awake[45], Is.False);
		}

		[Test]
		public void GetSleepTime()
		{
			var shift = new GuardShift(DateTime.Now, 1);

			shift.Awake[2] = false;
			shift.Awake[3] = false;
			shift.Awake[5] = false;

			var result = shift.GetSleepTime();

			Assert.That(result, Is.EqualTo(3));
		}

		[Test]
		public void GetTotalSleepTimePerGuard()
		{
			var sleepTimes = GuardShift.GetTotalSleepTimesPerGuard(s_shifts);

			Assert.That(sleepTimes.Keys.Count, Is.EqualTo(2));
			Assert.That(sleepTimes[10], Is.EqualTo(50));
			Assert.That(sleepTimes[99], Is.EqualTo(30));
		}

		[Test]
		public void FindSleepiestGuard()
		{
			var sleepTimes = GuardShift.GetTotalSleepTimesPerGuard(s_shifts);

			var guardId = GuardShift.FindSleepiestGuard(sleepTimes);

			Assert.That(guardId, Is.EqualTo(10));
		}

		[Test]
		public void FindMostSleptMinute()
		{
			var result = GuardShift.FindMostSleptMinute(s_shifts.Where(s => s.GuardId == 10).ToArray());
			Assert.That(result, Is.EqualTo(24));
		}
	}
}
