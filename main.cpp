#include <vector>
#include <string>
#include <queue>
#include <benchmark/benchmark.h>

static void VectorInsert(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::vector<std::string> vec;
        for(size_t i = 0; i < state.range(0); i++)
        {
            vec.insert(vec.end(), "test");
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

static void VectorPushback(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::vector<std::string> vec;
        for(size_t i = 0; i < state.range(0); i++)
        {
            vec.push_back("test");
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

static void VectorEmplaceback(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::vector<std::string> vec;
        for(size_t i = 0; i < state.range(0); i++)
        {
            vec.emplace_back("test");
        }
        benchmark::DoNotOptimize(vec.data());
        benchmark::ClobberMemory();
    }
}

BENCHMARK(VectorInsert)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});
BENCHMARK(VectorPushback)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});
BENCHMARK(VectorEmplaceback)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});

static void QueuePush(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::queue<std::string> q;
        for(size_t i = 0; i < state.range(0); i++)
        {
            q.push("test");
        }
        benchmark::DoNotOptimize(q);
        benchmark::ClobberMemory();
    }
}

static void QueueEmplace(benchmark::State& state)
{
    for(auto _ : state)
    {
        std::queue<std::string> q;
        for(size_t i = 0; i < state.range(0); i++)
        {
            q.emplace("test");
        }
        benchmark::DoNotOptimize(q);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(QueuePush)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});
BENCHMARK(QueueEmplace)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});

static void VectorIterate(benchmark::State& state)
{
    std::vector<std::string> q;
    for(size_t i = 0; i < state.range(0); i++)
    {
        q.emplace_back("123");
    }

    for(auto _ : state)
    {
        std::vector<std::string> temp = q;
        for(const auto& data : temp)
        {
            std::string t = data;
            benchmark::DoNotOptimize(temp.data());
            benchmark::DoNotOptimize(t);
            benchmark::ClobberMemory();
        }
    }
}

static void QueueIterate(benchmark::State& state)
{
    std::queue<std::string> q;
    for(size_t i = 0; i < state.range(0); i++)
    {
        q.emplace("123");
    }

    for(auto _ : state)
    {
        std::queue<std::string> temp = q;
        while(!temp.empty())
        {
            std::string t = temp.back();
            temp.pop();
            benchmark::DoNotOptimize(temp);
            benchmark::DoNotOptimize(t);
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(VectorIterate)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});
BENCHMARK(QueueIterate)->Args({1 << 4})->Args({1 << 12})->Args({1 << 13})->Args({1 << 14})->Args({1 << 15})->Args({1 << 16});
