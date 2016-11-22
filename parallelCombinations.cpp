#include <vector>
#include <cstdint>
#include <iostream>
#include <array>

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
    // std::size_t n = 5;
    // constexpr std::size_t k = 3;
    const std::size_t lastElement = k - 1;

    std::vector<int> currentCombination(k);
    int i;

    int fuck = 0;

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

        static int count = 0;
        cout << count++ << ":";
        for(auto i : currentCombination)
            cout << i;
        cout << '\n';
        fuck++;

    } while (currentCombination[0] != (n-k) || currentCombination[lastElement] != (n-1));

    cout << "num combinations: " << fuck << endl;
}

int main() {

	const int n = 10;
	const int k = 3;
	const int m = 100;
	generateCombos(n, k);

	cout << numCombinations(n, k) << '\n';
	printMthComb(m, n, k);
}