#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <benchmark/benchmark.h>

template <class T>
	static T str2(const std::string& str)
	{
		T tmp;
		std::stringstream ss(str, std::stringstream::in);
		ss >> tmp;
		return (tmp);
	}

static long tmToUnix(tm tm)
{

	tm.tm_isdst = -1;
	/*
	 * Not set by strptime(); tells mktime()
	 * to determine whether daylight saving time
	 * is in effect
	 **/
	time_t t = mktime(&tm);
	if (-1 == t)
	{
		return (0);
	}
	return ((long)t);
}

static long DatetimeToUnix(const std::string& date)
{
	struct tm tm;
	strptime(date.c_str(), "%Y-%m-%d%n%T", &tm);
	return (timegm(&tm));
}

static long GetUnixTime()
{
	return static_cast<long>(time(nullptr));
}

static long GetTimezoneShift(time_t when = -1)
{
	if (when == -1)
		when = std::time(nullptr);
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	// s is in ISO 8601 format: "±HHMM"
	int h = std::stoi(s.substr(0, 3), nullptr, 10);
	int m = std::stoi(s[0] + s.substr(3), nullptr, 10);

	return h * 3600 + m * 60;
}

static std::string UnixToDatetime(long unixTime)
{
	time_t sec = (time_t)unixTime;
	sec -= GetTimezoneShift();
	struct tm* timeptr = localtime(&sec);

	char result[200];
	sprintf(result, "%d-%.2d-%.2d %.2d:%.2d:%.2d", 1900 + timeptr->tm_year, timeptr->tm_mon, timeptr->tm_mday, timeptr->tm_hour,
			timeptr->tm_min, timeptr->tm_sec);

	return (std::string(result));
}

static std::string UnixToLocaltime(long unixTime)
{
	time_t sec = (time_t)unixTime;
	struct tm* timeinfo = localtime(&sec);
	std::string s = asctime(timeinfo);
	return (s.substr(0, s.size() - 1));
}

static long LocaltimeToUnix(const std::string& local)
{
	struct tm tm;
	strptime(local.c_str(), "%n%a%n%b%n%d%n%T%n%Y%n", &tm);
	return (tmToUnix(tm) - GetTimezoneShift());
}

static std::string DatetimeToLocaltime(const std::string& date)
{
	auto dateUnix = DatetimeToUnix(date);
	return UnixToLocaltime(dateUnix);
}

static std::string LocaltimeToDatetime(const std::string& local)
{
	auto localUnix = LocaltimeToUnix(local);
	return UnixToDatetime(localUnix);
}

static std::string UnixToUTC(long unixTime)
{
	time_t sec = (time_t)unixTime;
	sec -= GetTimezoneShift();
	struct tm* timeptr = localtime(&sec);
	static const char wday_name[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	static const char mon_name[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	static char result[100];
	sprintf(result, "%.3s %.3s %2d %.2d:%.2d:%.2d %d", wday_name[timeptr->tm_wday], mon_name[timeptr->tm_mon], timeptr->tm_mday,
			timeptr->tm_hour, timeptr->tm_min, timeptr->tm_sec, 1900 + timeptr->tm_year);
	return result;
}

static std::string LocaltimeToUTC(const std::string& local)
{
	auto localUnix = LocaltimeToUnix(local) + GetTimezoneShift();
	return UnixToUTC(localUnix);
}

static long UTCtoUnix(const std::string& utc)
{
	struct tm tm;
	strptime(utc.c_str(), "%n%a%n%b%n%d%n%T%n%Y%n", &tm);
	return (timegm(&tm));
}

static std::string rtrim(const std::string& str)
{
	size_t pos = str.find_last_not_of(" \t\n\r");
	if (std::string::npos == pos)
	{
		return ("");
	}
	return (str.substr(0, pos + 1));
}

static std::string ltrim(const std::string& str)
{
	size_t pos = str.find_first_not_of(" \t\n\r");
	if (std::string::npos == pos)
	{
		return ("");
	}
	return (str.substr(pos));
}

static std::string trim(const std::string& str)
{
	return (rtrim(ltrim(str)));
}

template <class T>
static void split(const std::string& str, const std::string& delim, T* results)
{
    size_t cutAt;
    std::string token, temp = str;
    while ((cutAt = temp.find(delim)) != std::string::npos)
    {
        token = temp.substr(0, cutAt);
        if (token != "")
        {
            results->insert(results->end(), token);
        }
        temp = temp.substr(cutAt + delim.length());
    }
    if (trim(temp).size() != 0)
    {
        results->insert(results->end(), temp);
    }
}

static long WallToSeconds(const std::string& wall)
{
	unsigned int seconds = 0;
	std::vector<std::string> vec;

	split<std::vector<std::string>>(wall, ":", &vec);

	unsigned int vec_size = vec.size();

	if (0 < vec_size)
	{
		seconds += (str2<unsigned int>(vec.at(vec_size - 1)));
		if (1 < vec_size)
		{
			seconds += (60 * str2<unsigned int>(vec.at(vec_size - 2)));
			if (2 < vec_size)
			{
				seconds += (3600 * str2<unsigned int>(vec.at(vec_size - 3)));
				if (3 < vec_size)
				{
					seconds += (86400 * str2<unsigned int>(vec.at(vec_size - 4)));
				}
			}
		}
	}
	return (seconds);
}

static void BM_WallToSeconds(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto a = WallToSeconds("18:25:52");
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

static void BM_UTCtoUnix(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto a = UTCtoUnix("Mon Feb 31 20:10:35 2012");
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

static void BM_UnixToUTC(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto a = UnixToUTC(6812724793);
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

static void BM_GetUnixTime(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto a = GetUnixTime();
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

static void BM_TimeZoneShift(benchmark::State& state)
{
    for(auto _ : state)
    {
        auto a = GetTimezoneShift();
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_GetUnixTime);
BENCHMARK(BM_UnixToUTC);
BENCHMARK(BM_WallToSeconds);
BENCHMARK(BM_UTCtoUnix);
BENCHMARK(BM_TimeZoneShift);