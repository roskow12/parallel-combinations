#include <vector>
#include <list>
#include <cstdint>
#include <memory>
#include <iostream>
#include <array>
#include <chrono>
#include <limits>
#include <future>
#include <thread>
#include "parallelCombinations.h"


const uint8_t NUM_THREADS = 4;

using namespace std::chrono;
using std::cout;
using std::endl;

uint8_t largestV(const uint8_t a, const uint8_t b, const uint64_t x) {
	uint8_t v = a-1;

	while(Combination::numCombinations(v,b) > x)
		--v;

	return v;
}

std::vector<uint8_t> mthComb(const uint64_t m, const uint8_t n, const uint8_t k) {
	std::vector<uint8_t> ans(k, 0);

	uint8_t a = n;
	uint8_t b = k;

	uint64_t x = (Combination::numCombinations(n, k) - 1) - m;

	for(uint8_t i=0; i<k; ++i) {
		ans[i] = largestV(a, b, x);
		x = x - Combination::numCombinations(ans[i], b);
		a = ans[i];
		b = b-1;
	}

	for(uint8_t i=0; i<k; ++i) {
		ans[i] = (n-1) - ans[i];
	}

	cout << m << "th combination: ";
	for(auto e : ans)
		cout << unsigned(e) << ' ';
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

	uint64_t count = 0;
	while (comb[0] != (n-k) || comb[lastElement] != (n-1)) {
		next_combination(comb, n, k);
		++count;
	}
	cout << "legacy count:\t" << count << '\n';

	delete[] comb;
}

uint64_t generateCombos3(const int n, const int k) {
	
	Combination c(n,k);

	return c.generate();
}

uint64_t generateCombos4(const int n, const int k) {
	const uint64_t numComb = Combination::numCombinations(n, k);

	const uint64_t quarter = (uint64_t) numComb / 4;
	const uint64_t remainder =  numComb % 4;

	const std::unique_ptr<Combination> a = std::make_unique<Combination>(n, k, quarter+1);
	const std::unique_ptr<Combination> b = std::make_unique<Combination>(n, k, quarter, mthComb(quarter,n,k));
	const std::unique_ptr<Combination> c = std::make_unique<Combination>(n, k, quarter, mthComb(quarter*2,n,k));
	const std::unique_ptr<Combination> d = std::make_unique<Combination>(n, k, quarter+remainder-1, mthComb(quarter*3,n,k));

	auto f1 = std::async(std::launch::async, &Combination::generate, a.get());
	auto f2 = std::async(std::launch::async, &Combination::generate, b.get());
	auto f3 = std::async(std::launch::async, &Combination::generate, c.get());
	auto f4 = std::async(std::launch::async, &Combination::generate, d.get());

	uint64_t total = f1.get();
	cout << "f1" << endl;
	total += f2.get();
	cout << "f2" << endl;
	total += f3.get();
	cout << "f3" << endl;
	total += f4.get();
	cout << "f4" << endl;

	return total;
}

uint64_t generateCombos5(const int n, const int k) {
	const uint64_t numComb = Combination::numCombinations(n, k);

	const uint64_t eighth = (uint64_t)numComb / 6;
	const uint64_t remainder = numComb % 6;

	const std::unique_ptr<Combination> a = std::make_unique<Combination>(n, k, eighth + 1);
	const std::unique_ptr<Combination> b = std::make_unique<Combination>(n, k, eighth, mthComb(eighth, n, k));
	const std::unique_ptr<Combination> c = std::make_unique<Combination>(n, k, eighth, mthComb(eighth * 2, n, k));
	const std::unique_ptr<Combination> d = std::make_unique<Combination>(n, k, eighth, mthComb(eighth * 3, n, k));
	const std::unique_ptr<Combination> e = std::make_unique<Combination>(n, k, eighth, mthComb(eighth * 4, n, k));
	const std::unique_ptr<Combination> f = std::make_unique<Combination>(n, k, eighth + remainder - 1, mthComb(eighth * 5, n, k));

	auto f1 = std::async(std::launch::async, &Combination::generate, a.get());
	auto f2 = std::async(std::launch::async, &Combination::generate, b.get());
	auto f3 = std::async(std::launch::async, &Combination::generate, c.get());
	auto f4 = std::async(std::launch::async, &Combination::generate, d.get());
	auto f5 = std::async(std::launch::async, &Combination::generate, e.get());
	auto f6 = std::async(std::launch::async, &Combination::generate, f.get());


	uint64_t total = f1.get();
	cout << "f1" << endl;
	total += f2.get();
	cout << "f2" << endl;
	total += f3.get();
	cout << "f3" << endl;
	total += f4.get();
	cout << "f4" << endl;
	total += f5.get();
	cout << "f5" << endl;
	total += f6.get();
	cout << "f6" << endl;


	return total;
}

uint64_t generateCombos6(const int n, const int k) {
	const uint64_t numComb = Combination::numCombinations(n, k);

	const uint64_t chunkSize = (uint64_t)numComb / NUM_THREADS;
	const uint64_t remainder = numComb % NUM_THREADS;

	std::vector<std::unique_ptr<Combination>> combs;
	combs.push_back(std::make_unique<Combination>(n, k, chunkSize+1));
	for (int i = 1; i < NUM_THREADS - 1; ++i)
		combs.push_back(std::make_unique<Combination>(n, k, chunkSize, mthComb(chunkSize*i, n, k)));
	combs.push_back(std::make_unique<Combination>(n, k, chunkSize + remainder - 1, mthComb(chunkSize*(NUM_THREADS-1), n, k)));

	std::list<std::future<uint64_t>> futures;
	for (int i = 0; i < NUM_THREADS; ++i)
		futures.push_back(std::async(std::launch::async, &Combination::generate, combs.at(i).get()));

	uint64_t total = 0;
	for (auto it = futures.begin(); it != futures.end(); ++it) {
		total += (*it).get();
		cout << total << '\n';
	}
		
	return total;
}

int main(int argc, const char* argv[]) {
	cout << "# args: " << argc << '\n';

	const int n = atoi(argv[1]);
	const int k = atoi(argv[2]);	

	cout << "actual count:\t" << Combination::numCombinations(n, k) << '\n';

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	cout << "legacy count:\t" << generateCombos4(n, k) << '\n';
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	auto duration0 = duration_cast<microseconds>(t2-t1).count();
	cout << "legacy time:\t" << duration0 << '\n';

	t1 = high_resolution_clock::now();
	cout << "new count:\t" << generateCombos5(n, k) << '\n';
	t2 = high_resolution_clock::now();

	auto duration1 = duration_cast<microseconds>(t2-t1).count();
	cout << "new time:\t" << duration1 << '\n';

	return 0;
}