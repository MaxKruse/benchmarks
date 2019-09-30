#include <benchmark/benchmark.h>
#include <string>

static std::string replace(const std::string& str, const std::string& a, const std::string& b)
{
	std::string temp = str;
	size_t pos = 0, bsize = b.size(), asize = a.size();
	while (std::string::npos != (pos = temp.find(a, pos)))
	{
		temp.replace(pos, asize, b);
		pos += bsize;
	}
	return (temp);
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

static std::string str2lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return (s);
}

static std::string str2upper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	return (s);
}

static unsigned substr_count(const std::string& str, const std::string& sub)
{
	size_t c = 0, i = 0;
	for (; std::string::npos != (c = str.find(sub, c)); ++i, ++c)
		;
	return (i);
}

static std::string lsplit(const std::string& str, const std::string& del)
{
	size_t pos = str.find(del);
	if (std::string::npos == pos)
	{
		return (str);
	}
	return (str.substr(0, pos));
}

static std::string rsplit(const std::string& str, const std::string& del)
{
	size_t pos = str.find(del);
	if (std::string::npos == pos)
	{
		// Wenn rsplit nichts findet, ist pos am ende von str,
		// und rechts von pos waere dann nichts.
		return ("");
	}
	return (str.substr(pos + del.size()));
}

static std::string reverse_lsplit(const std::string& str, const std::string& del)
{
	size_t pos = str.rfind(del);
	if (std::string::npos == pos)
	{
		// Wenn reverse_lsplit nichts findet, ist pos am Anfang von str,
		// und links von pos waere dann nichts.
		return ("");
	}
	return (str.substr(0, pos));
}

static std::string reverse_rsplit(const std::string& str, const std::string& del)
{
	size_t pos = str.rfind(del);
	if (std::string::npos == pos)
	{
		// Wenn rsplit nichts findet, ist pos Anfang von str,
		// und rechts das ganze str.
		return (str);
	}
	return (str.substr(pos + del.size()));
}

static std::string replace_sequence(const std::string& str, const std::string& beg, const std::string& end, const std::string& in,
										  size_t* skip)
{
	const size_t begin_pos = str.find(beg, *skip);
	*skip = begin_pos;
	if (std::string::npos == begin_pos)
	{
		return (str);
	}

	const size_t end_pos = str.find(end, begin_pos + 1);
	if (std::string::npos == end_pos)
	{
		*skip = std::string::npos;
		return (str);
	}
	*skip += in.size();
	return (str.substr(0, begin_pos) + in + str.substr(end_pos + 1, str.size()));
}

static std::string replace_sequence(const std::string& str, const std::string& beg, const std::string& end, const std::string& in)
{
	size_t skip = 0;
	return (replace_sequence(str, beg, end, in, &skip));
}

static std::string replace_sequence_all(const std::string& str, const std::string& beg, const std::string& end, const std::string& in)
{
	size_t skipper = 0;
	std::string temp = str;
	do
	{
		temp = replace_sequence(temp, beg, end, in, &skipper);
	} while (skipper != std::string::npos);
	return (temp);
}

static bool issubstr(const std::string& str, const std::string& sub)
{
	return (std::string::npos != str.find(sub));
}


static void BM_IsSubStr(benchmark::State& state)
{
    for(auto _ : state)
    {
        bool b = issubstr("Hello this is a string", "is");
        benchmark::DoNotOptimize(b);
        benchmark::ClobberMemory();
    }
}

static void BM_replace_sequence_all(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = replace_sequence_all("lala test test lala 123 lala test 123 lala", "123", "lala", "kartoffel");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_replace_sequence(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = replace_sequence("lala test test lala 123 lala test 123 lala", "123", "lala", "kartoffel");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_reverse_rsplit(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = reverse_rsplit("lala test test lala 123 lala test 123 lala", "123");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_reverse_lsplit(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = reverse_lsplit("lala test test lala 123 lala test 123 lala", "123");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_lsplit(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = lsplit("lala test test lala 123 lala test 123 lala", "123");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_rsplit(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = rsplit("lala test test lala 123 lala test 123 lala", "123");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_substr_count(benchmark::State& state)
{
    for(auto _ : state)
    {
        size_t r = substr_count("lala test test lala 123 lala test 123 lala", "123");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_strUpper(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = str2upper("lala test test lala 123 lala test 123 lala");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_strLower(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = str2lower("lala test test lala 123 lala test 123 lala");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_trim(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = trim("lala test test lala 123 lala test 123 lala");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}

static void BM_replace(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::string r = replace("lala test test lala 123 lala test 123 lala", "123", "kartoffel");
        
        benchmark::DoNotOptimize(r);
        benchmark::ClobberMemory();
    }
}


BENCHMARK(BM_IsSubStr);
BENCHMARK(BM_replace_sequence_all);
BENCHMARK(BM_replace_sequence);
BENCHMARK(BM_reverse_rsplit);
BENCHMARK(BM_reverse_lsplit);
BENCHMARK(BM_lsplit);
BENCHMARK(BM_rsplit);
BENCHMARK(BM_substr_count);
BENCHMARK(BM_strUpper);
BENCHMARK(BM_strLower);
BENCHMARK(BM_trim);
BENCHMARK(BM_replace);
