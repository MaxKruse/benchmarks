#include <benchmark/benchmark.h>
#include <map>
#include <string>

static std::string OldLookupT(std::string key, std::map<std::string, std::string>& cache)
{
    if(cache[key] != std::string())
        return cache[key];

    cache[key] = "a";
    return cache[key];
}

static std::string NewLookupT(std::string key, std::map<std::string, std::string>& cache)
{
    std::string &e = cache[key];
    if(e != std::string())
        return cache[key];

    e = "a";
    return e;
}


static void OldLookup(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::map<std::string, std::string> m;
        m["test"] = "yes";
        auto a = OldLookupT("test", m);
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

static void NewLookup(benchmark::State& state)
{
    std::map<std::string, std::string> m;
    m["test"] = "yes";

    for(auto _ : state)
    {
        std::map<std::string, std::string> m;
        m["test"] = "yes";
        auto a = NewLookupT("test", m);
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(NewLookup);
BENCHMARK(OldLookup);