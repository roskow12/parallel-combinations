#include "parallelCombinations.h"

Combination::Combination(const uint8_t n, const uint8_t k) :
	n(n), 
	k(k), 
	lastIndex(k-1), 
	k_minus_2(k-2), 
	n_minus_1(n-1),
	n_minus_k(n-k),
	combsToCompute(numCombinations(n, k)), 
	currentCombination(new uint8_t[k]),
	currentCombIndex(0)
{
	for(int i=0; i<k; ++i)
		currentCombination[i] = i;
	currentCombination[lastIndex] = k_minus_2;
}

Combination::Combination(const uint8_t n, const uint8_t k,
		const uint8_t* startCombination, const long combsToCompute) :
	n(n), 
	k(k), 
	lastIndex(k-1), 
	k_minus_2(k-2), 
	n_minus_1(n-1),
	n_minus_k(n-k),
	combsToCompute(combsToCompute), 
	currentCombination(new uint8_t[k]),
	currentCombIndex(0)
{
	for(int i=0; i<k; ++i)
		currentCombination[i] = startCombination[i];
}

Combination::~Combination() {
	delete[] combsToCompute;
}

bool Combination::next(){
	if(++currentCombIndex > combsToCompute)
		return false;

	if (currentCombination[lastIndex] == n_minus_1) {
		int i = k_minus_2;
		while (currentCombination[i] == (n_minus_k+i))
            i--;

        currentCombination[i]++;

        for(int j=(i+1); j<k; ++j)
        	currentCombination[j] = currentCombination[i]+j-i;
	}
	else
		currentCombination[lastIndex]++;

	return true;
}

long Combination::numCombinations(const uint8_t n, const uint8_t k) {
	if(n<k)
		return 0;
	else if(n == k)
		return 1;

	long delta, iMax;

	const uint8_t nMinusK = n-k;

	if(k < nMinusK){
		delta = nMinusK;
		iMax = k;
	}
	else{
		delta = k;
		iMax = nMinusK;
	}

	long ans = delta + 1;

	for(long i=2; i <= iMax; ++i)
		ans = (ans * (delta+i)) / i;

	return ans;
}