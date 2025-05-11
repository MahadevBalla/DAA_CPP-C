#include <iostream>
#include <climits>
#include <string>
#include <time.h>
using namespace std;

// Rabin-Karp string search algorithm
void search(string text, string pattern, int q) {
	int M = pattern.size();
	int N = text.size();
	bool found = false;
	int h = 1;
	int d = 256;	// number of characters in input alphabet

	clock_t start, end;
    double time_spent;
	
	start = clock(); // start timer

	// Calculate h = d^(m-1) % q
	for(int i = 0; i < M-1; i++) {
		h = (h*d)%q;
	}

	int t = 0;	// hash value for text
	int p = 0;	// hash value for pattern
	for(int i = 0; i < M; i++) {
		p = ((p*d) + pattern[i])%q;
		t = ((t*d) + text[i])%q;
	}
	
	// Slide pattern over text
	for(int i = 0; i <= N - M; i++) {
		if(p == t) {
			// If hash matches, check characters
			if(!pattern.compare(text.substr(i, M)))
				// If pattern fully matched
				cout << "Pattern found at: " << i << endl;
				found = true;
		}

		// Calculate hash for next window
		if(i < N-M) {
			t = (d*(t - text[i]*h) + text[M+i]) % q;
			
			// Make sure hash is positive
			if (t < 0)
              t = (t + q);
		}
	}

	end = clock(); // end timer
	time_spent = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
    
    printf("Time taken: %.3f ms\n", time_spent);
    if (!found)
        printf("Pattern not found in text.\n");
}

int main() {
	string text, pattern;
	int q = INT_MAX;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter pattern: ";
    getline(cin, pattern);

	search(text, pattern, q);

	return 0;
}