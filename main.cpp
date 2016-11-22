#include <vector>
#include <cstdint>
#include <iostream>
#include <array>
#include <chrono>
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

void printMthComb(const int m, const int n, const int k) {
	std::vector<int> ans(k, 0);

	int a = n;
	int b = k;

	long x = (numCombinations(n, k) - 1) - m;

	for(int i=0; i<k; ++i) {
		ans[i] = largestV(a, b, x);
		x = x - numCombinations(ans[i], b);
		a = ans[i];
		b = b-1;
	}

	for(int i=0; i<k; ++i) {
		ans[i] = (n-1) - ans[i];
	}

	cout << m << "th combination: ";
	for(auto e : ans)
		cout << e;
	cout << '\n';
}

//n-choose-k
void generateCombos(const int n, const int k) {
    const std::size_t lastElement = k - 1;

    std::vector<int> currentCombination(k);
    int i;

    long fuck = 0;

    // fill initial combination is real first combination -1 for last number, 
    // as we will increase it in loop
    for(i=0; i<k; i++) {
        currentCombination[i]=i;    
    }
    currentCombination[lastElement] = k-2; 
    
    do
    {
        if (currentCombination[lastElement] == (n-1) ) // if last number is just before overwhelm
        {
            int i = k-2;
            while (currentCombination[i] == (n-k+i))
                i--;

            currentCombination[i]++;

            for (int j=(i+1); j<k; j++)
                currentCombination[j] = currentCombination[i]+j-i;
        }
        else
            currentCombination[lastElement]++;

        // static int count = 0;
        // cout << count++ << ":";
        // for(auto i : currentCombination)
        //     cout << i;
        // cout << '\n';
        fuck++;

    } while (currentCombination[0] != (n-k) || currentCombination[lastElement] != (n-1));

    cout << "legacy count: " << fuck << endl;
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

void first_combination(uint8_t* currentCombination, const int n, const int k) {
	for(int i = 0; i < k; ++i)
		currentCombination[i] = i;
	currentCombination[k-1] = k-2;
}

void generateCombos2(const int n, const int k) {
	const int lastElement = k - 1;
	auto* comb = new uint8_t[k];
	first_combination(comb, n, k);

	long count = 0;
	while (comb[0] != (n-k) || comb[lastElement] != (n-1)) {
		next_combination(comb, n, k);
		++count;
	}
	cout << "legacy count:\t" << count << '\n';

	delete[] comb;
}

void generateCombos3(const int n, const int k) {
	Combination c(n,k);

	long count = 0;
	while(c.next())
		++count;
	cout << "new count:\t" << count << '\n';
}

int main() {

	const int n = 60;
	const int k = 8;
	// const int m = 100;
	

	cout << "actual count:\t" << numCombinations(n, k) << '\n';
	//printMthComb(m, n, k);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	generateCombos2(n, k);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	auto duration0 = duration_cast<microseconds>(t2-t1).count();
	cout << "Legacy:\t" << duration0 << '\n';

	t1 = high_resolution_clock::now();
	generateCombos3(n, k);
	t2 = high_resolution_clock::now();

	auto duration1 = duration_cast<microseconds>(t2-t1).count();
	cout << "New:\t" << duration1 << '\n';
}