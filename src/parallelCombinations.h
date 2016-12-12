#ifndef _PARALLEL_COMB_H_
#define _PARALLEL_COMB_H_

#include "combDefines.h"
#include <cstdint>
#include <vector>

class Combination {
  const uint64_t m_combsToCompute;
  uint64_t count;

public:
  std::array<uint8_t, K> currentCombination;

  bool next() {
    if (++count > m_combsToCompute)
      return false;

    if (currentCombination[LAST_INDEX] == N_MINUS_1) {
      uint8_t i = K_MINUS_2;
      while (currentCombination[i] == (N_MINUS_K + i))
        i--;

      currentCombination[i]++;

      for (uint8_t j = (i + 1); j < K; ++j)
        currentCombination[j] = currentCombination[i] + j - i;
    } else
      currentCombination[LAST_INDEX]++;

    return true;
  }

  uint64_t generate() {
    while (this->next()) {
    }
    return count - 1;
  }

  Combination(const uint64_t combsToCompute = 0)
      : m_combsToCompute((combsToCompute == 0) ? numCombinations(N, K)
                                               : combsToCompute),
        count(0) {
    for (int i = 0; i < K; ++i)
      currentCombination[i] = i;
    currentCombination[LAST_INDEX] = K_MINUS_2;
  }

  Combination(const uint64_t combsToCompute,
              const std::vector<uint8_t> startCombination)
      : m_combsToCompute(combsToCompute), 
        count(0) {
    for (int i = 0; i < K; ++i)
      currentCombination[i] = startCombination[i];
  }

  ~Combination() {}

  static constexpr uint64_t numCombinations() {

    if (N < K)
      return 0;
    else if (N == K)
      return 1;

    uint64_t delta, iMax;


    if (K < N_MINUS_K) {
      delta = N_MINUS_K;
      iMax = K;
    } else {
      delta = K;
      iMax = N_MINUS_K;
    }

    uint64_t ans = delta + 1;

    for (uint64_t i = 2; i <= iMax; ++i)
      ans = (ans * (delta + i)) / i;

    return ans;
  }
};



#endif //_PARALLEL_COMB_H_