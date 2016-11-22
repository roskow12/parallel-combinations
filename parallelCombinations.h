#ifndef _PARALLEL_COMB_H_
#define _PARALLEL_COMB_H_

#include <cstdint>

class Combination {
	const uint8_t n;
	const uint8_t k;
	const uint8_t lastIndex;
	const uint8_t k_minus_2;
	const uint8_t n_minus_1;
	const uint8_t n_minus_k;
	const long combsToCompute;
	uint8_t* currentCombination;
	long currentCombIndex;

public:
	Combination(const uint8_t n, const uint8_t k);
	Combination(const uint8_t n, const uint8_t k, 
		const uint8_t* startCombination, const long combsToCompute);

	bool next();

	static long numCombinations(const uint8_t n, const uint8_t k);
};

#endif //_PARALLEL_COMB_H_