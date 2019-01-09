#include <benchmark/benchmark.h>

void SimpleTest(benchmark::State& st) {
  while (st.KeepRunning() ) {
    float a = 3;
  }
}

BENCHMARK(SimpleTest);