#ifndef _PARALLEL_COMB_H_
#define _PARALLEL_COMB_H_

#include <cstdint>
#include <vector>

class Combination {
  const uint8_t k;
  const uint8_t lastIndex;
  const uint8_t k_minus_2;
  const uint8_t n_minus_1;
  const uint8_t n_minus_k;
  const uint64_t m_combsToCompute;
  uint64_t count;

public:
  std::array<uint8_t, 8> currentCombination;

  bool next() {
    if (++count > m_combsToCompute)
      return false;

    if (currentCombination[lastIndex] == n_minus_1) {
      uint8_t i = k_minus_2;
      while (currentCombination[i] == (n_minus_k + i))
        i--;

      currentCombination[i]++;

      for (uint8_t j = (i + 1); j < k; ++j)
        currentCombination[j] = currentCombination[i] + j - i;
    } else
      currentCombination[lastIndex]++;

    return true;
  }

  uint64_t generate() {
    while (this->next()) {
    }
    return count - 1;
  }

  Combination(const uint8_t n, const uint8_t k,
              const uint64_t combsToCompute = 0)
      : k(k), 
      	lastIndex(k - 1), 	
      	k_minus_2(k - 2), 	
      	n_minus_1(n - 1),
        n_minus_k(n - k),
        m_combsToCompute((combsToCompute == 0) ? numCombinations(n, k)
                                               : combsToCompute),
        count(0) {
    for (int i = 0; i < k; ++i)
      currentCombination[i] = i;
    currentCombination[lastIndex] = k_minus_2;
  }

  Combination(const uint8_t n, const uint8_t k, const uint64_t combsToCompute,
              const std::vector<uint8_t> startCombination)
      : k(k), 
      	lastIndex(k - 1), 
  		k_minus_2(k - 2), 
  		n_minus_1(n - 1),
        n_minus_k(n - k), 
        m_combsToCompute(combsToCompute), 
        count(0) {
    for (int i = 0; i < k; ++i)
      currentCombination[i] = startCombination[i];
  }

  ~Combination() {}

  static uint64_t numCombinations(const long n, const long k) {

    if (n < k)
      return 0;
    else if (n == k)
      return 1;

    uint64_t delta, iMax;

    const int nMinusK = n - k;

    if (k < nMinusK) {
      delta = nMinusK;
      iMax = k;
    } else {
      delta = k;
      iMax = nMinusK;
    }

    uint64_t ans = delta + 1;

    for (uint64_t i = 2; i <= iMax; ++i)
      ans = (ans * (delta + i)) / i;

    return ans;
  }
};



#endif //_PARALLEL_COMB_H_