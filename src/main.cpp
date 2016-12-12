#include "parallelCombinations.h"
#include "combDefines.h"
#include "microbench/microbench.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <future>
#include <iomanip>
#include <memory>
#include <thread>
#include <vector>
#include <iostream>
#include <list>

// #define DEBUG_PRINTS

// const uint8_t NUM_THREADS = 4;

using namespace std::chrono;
using std::cout;
using std::endl;

uint64_t acutalNum = 0;

uint8_t largestV(const uint8_t a, const uint8_t b, const uint64_t x) {
  uint8_t v = a - 1;

  while (Combination::numCombinations(v, b) > x)
    --v;

  return v;
}

std::vector<uint8_t> mthComb(const uint64_t m, const uint8_t n,
                             const uint8_t k) {
  std::vector<uint8_t> ans(k, 0);

  uint8_t a = n;
  uint8_t b = k;

  uint64_t x = (Combination::numCombinations(n, k) - 1) - m;

  for (uint8_t i = 0; i < k; ++i) {
    ans[i] = largestV(a, b, x);
    x = x - Combination::numCombinations(ans[i], b);
    a = ans[i];
    b = b - 1;
  }

  for (uint8_t i = 0; i < k; ++i) {
    ans[i] = (n - 1) - ans[i];
  }
#ifdef DEBUG_PRINTS
  std::cout << m << "th combination: ";
  for (auto e : ans)
    cout << unsigned(e) << ' ';
  cout << '\n';
#endif
  return ans;
}

uint64_t generateCombos3(const int n, const int k) {

  Combination c(n, k);

  uint64_t total = c.generate();

  if (total != acutalNum)
    printf("Wrong count computed!\n");

  return total;
}

uint64_t generateCombos6(const int n, const int k) {
  const uint64_t numComb = Combination::numCombinations(n, k);

  const uint64_t chunkSize = (uint64_t)numComb / NUM_THREADS;
  const uint64_t remainder = numComb % NUM_THREADS;

  std::vector<std::unique_ptr<Combination>> combs;
  combs.push_back(std::make_unique<Combination>(n, k, chunkSize + 1));
  for (int i = 1; i < NUM_THREADS - 1; ++i){
    combs.push_back(std::make_unique<Combination>(
      n, k, chunkSize, mthComb(chunkSize * i, n, k)));
  }
  combs.push_back(std::make_unique<Combination>(
      n, k, chunkSize + remainder - 1,
      mthComb(chunkSize * (NUM_THREADS - 1), n, k)));

  std::list<std::future<uint64_t>> futures;
  for (int i = 0; i < NUM_THREADS; ++i)
    futures.push_back(std::async(std::launch::async, &Combination::generate,
                                 combs.at(i).get()));

  uint64_t total = 0;
  for (auto it = futures.begin(); it != futures.end(); ++it) {
    total += (*it).get();
#ifdef DEBUG_PRINTS
    cout << total << '\n';
#endif
  }
  if (total != acutalNum)
    printf("Wrong count computed!\n");
  return total;
}

constexpr std::array<std::unique_ptr<Combination>, NUM_THREADS> initCombs() {

}

int main(int argc, const char *argv[]) {
  
  constexpr std::array<std::unique_ptr<Combination>, NUM_THREADS> combs = {
    std::make_unique<Combination>()
  }

  return 0;
}