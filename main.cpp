#include <vector>
#include <cstdint>
#include <memory>
#include <iostream>
#include <array>
#include <chrono>
#include <limits>
#include <future>
#include <thread>
#include "parallelCombinations.h"


using namespace std::chrono;
using std::cout;
using std::endl;

long numCombinations(const long n, const long k) {
	
	if(n<k)
		return 0;
	else if(n == k)
		return 1;

	long delta, iMax;

	const int nMinusK = n-k;

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

long largestV(const long a, const long b, const long x) {
	long v = a-1;

	while(numCombinations(v,b) > x)
		--v;

	return v;
}

std::vector<uint8_t> mthComb(const long m, const uint8_t n, const uint8_t k) {
	std::vector<uint8_t> ans(k, 0);

	uint8_t a = n;
	uint8_t b = k;

	long x = (numCombinations(n, k) - 1) - m;

	for(uint8_t i=0; i<k; ++i) {
		ans[i] = largestV(a, b, x);
		x = x - numCombinations(ans[i], b);
		a = ans[i];
		b = b-1;
	}

	for(uint8_t i=0; i<k; ++i) {
		ans[i] = (n-1) - ans[i];
	}

	cout << m << "th combination: ";
	for(auto e : ans)
		cout << static_cast<int>(e) << ' ';
	cout << '\n';

	return ans;
}

void next_combination(uint8_t* currentCombination, const int n, const int k) {
	const int lastElement = k - 1;
	
	if (currentCombination[lastElement] == n-1) {
		int i = k-2;
		while (currentCombination[i] == (n-k+i))
            i--;

        currentCombination[i]++;

        for(int j=(i+1); j<k; ++j)
        	currentCombination[j] = currentCombination[i]+j-i;
	}
	else
		currentCombination[lastElement]++;
}

void first_combination(uint8_t* currentCombination, const int k) {
	for(int i = 0; i < k; ++i)
		currentCombination[i] = i;
	currentCombination[k-1] = k-2;
}

void generateCombos2(const int n, const int k) {
	const int lastElement = k - 1;
	auto* comb = new uint8_t[k];
	first_combination(comb, k);

	long count = 0;
	while (comb[0] != (n-k) || comb[lastElement] != (n-1)) {
		next_combination(comb, n, k);
		++count;
	}
	cout << "legacy count:\t" << count << '\n';

	delete[] comb;
}

long generateCombos3(const int n, const int k) {
	
	Combination c(n,k);

	return c.generate();
}

long generateCombos4(const int n, const int k) {
	const long numComb = numCombinations(n, k);

	const long quarter = (long) numComb / 4;
	const long remainder =  numComb % 4;

	const std::unique_ptr<Combination> a = std::make_unique<Combination>(n, k, quarter+1);
	const std::unique_ptr<Combination> b = std::make_unique<Combination>(n, k, quarter, mthComb(quarter,n,k));
	const std::unique_ptr<Combination> c = std::make_unique<Combination>(n, k, quarter, mthComb(quarter*2,n,k));
	const std::unique_ptr<Combination> d = std::make_unique<Combination>(n, k, quarter+remainder-1, mthComb(quarter*3,n,k));

	auto f1 = std::async(std::launch::async, &Combination::generate, a.get());
	auto f2 = std::async(std::launch::async, &Combination::generate, b.get());
	auto f3 = std::async(std::launch::async, &Combination::generate, c.get());
	auto f4 = std::async(std::launch::async, &Combination::generate, d.get());

	long total = f1.get();
	cout << "f1" << endl;
	total += f2.get();
	cout << "f2" << endl;
	total += f3.get();
	cout << "f3" << endl;
	total += f4.get();
	cout << "f4" << endl;

	return total;

}

int main(int argc, const char* argv[]) {

	cout << "# args: " << argc << '\n';
	cout << "\t\t" << std::numeric_limits<int>::max() << '\n';

	const int n = atoi(argv[1]);
	const int k = atoi(argv[2]);	

	cout << "actual count:\t" << numCombinations(n, k) << '\n';

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	cout << "legacy count:\t" << generateCombos3(n, k) << '\n';
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	auto duration0 = duration_cast<microseconds>(t2-t1).count();
	cout << "legacy time:\t" << duration0 << '\n';

	t1 = high_resolution_clock::now();
	cout << "new count:\t" << generateCombos4(n, k) << '\n';
	t2 = high_resolution_clock::now();

	auto duration1 = duration_cast<microseconds>(t2-t1).count();
	cout << "new time:\t" << duration1 << '\n';
}