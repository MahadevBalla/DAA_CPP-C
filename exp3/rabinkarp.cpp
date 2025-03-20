#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

#define d 256  // Base for hash computation (256 for ASCII characters, 10 for nos.)
#define q 101  // Prime number for modular arithmetic to reduce hash collisions

const int NUM_FILES = 10;   // Number of input text files
const int PATTERN_COUNT = 10; // Number of patterns per file
const vector<int> PATTERN_SIZES = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }; // Pattern sizes

vector<string> input_files(NUM_FILES); // Stores file names

// Function to pre-process text (remove spaces, newlines, and normalize)
string preprocess_text(const string& text) {
    string processed;
    for (char c : text) {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            continue; // Skip spaces, newlines, and tabs
        }
        processed += tolower(c); // Convert to lowercase
    }
    return processed;
}

// Function to generate and save text files with varying sizes (1K to 10K characters)
void generate_text_files() {
    ifstream kaggle_file("big.txt"); // file extracted from Kaggle dataset
    if (!kaggle_file) {
        cerr << "Error opening Kaggle dataset file!" << endl;
        return;
    }

    string text;
    getline(kaggle_file, text, '\0'); // Read entire dataset
    kaggle_file.close();

    if (text.size() < 10000) {
        cerr << "Dataset too small!" << endl;
        return;
    }

    for (int i = 1; i <= NUM_FILES; i++) {
        int size = i * 1000; // 1K to 10K
        string filename = "input_" + to_string(size) + ".txt";
        input_files[i - 1] = filename;

        ofstream file(filename);
        file << text.substr(0, size); // Save only first 'size' characters
        file.close();
    }
}

// Function to generate random or real patterns
vector<string> generate_patterns(const string& text) {
    vector<string> patterns;
    for (int size : PATTERN_SIZES) {
        if (rand() % 2 == 0) {
            // Real pattern: Extract a substring of given size from the text
            int start = rand() % (text.size() - size);
            patterns.push_back(text.substr(start, size));
        }
        else {
            // Spurious pattern: Generate a random string of given size
            string spurious(size, ' ');
            for (char& c : spurious) {
                c = 'A' + rand() % 26; // Random uppercase letter
            }
            patterns.push_back(spurious);
        }
    }
    return patterns;
}

// Rabin-Karp algorithm
int rabin_karp_matcher(const string& T, const string& P) {
    int n = T.length(); // Length of the text
    int m = P.length(); // Length of the pattern
    int h = 1; // Hash multiplier for rolling hash
    int p = 0; // Hash value for the pattern
    int t = 0; // Hash value for the current window in the text
    int match_count = 0; // Count of pattern matches in the text

    // Calculate h = d^(m-1) % q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // Calculate initial hash values for the pattern and the first window of the text
    for (int i = 0; i < m; i++) {
        p = (d * p + P[i]) % q;
        t = (d * t + T[i]) % q;
    }

    // Slide the pattern over the text one character at a time
    for (int s = 0; s <= n - m; s++) {
        // If hash values match, check character by character
        if (p == t) {
            if (T.substr(s, m) == P) {
                match_count++; // Increment match count if pattern matches
            }
        }

        // Calculate hash value for the next window
        if (s < n - m) {
            t = (d * (t - T[s] * h) + T[s + m]) % q;
            if (t < 0) t += q; // Ensure hash value is non-negative
        }
    }

    return match_count;
}

// Main execution
int main() {
    srand(time(0)); // Seed random number generator
    generate_text_files(); // Generate input files from Kaggle dataset

    ofstream log_file("timing_results.csv"); // Log file for timing results
    log_file << "PatternSize,InputFile,Text(Ts),Pattern(Ps),TimeTaken(ms)\n"; // CSV header

    for (const string& filename : input_files) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening " << filename << endl;
            continue;
        }

        // Read the entire text file into memory
        string text;
        getline(file, text, '\0');
        file.close();

        // Pre-process the text (remove spaces, newlines, and normalize)
        string processed_text = preprocess_text(text);

        // Generate 10 patterns (real and spurious)
        vector<string> patterns = generate_patterns(processed_text);

        // Search for each pattern in the text and measure time
        for (const string& pattern : patterns) {
            auto start = high_resolution_clock::now(); // Start timer
            int matches = rabin_karp_matcher(processed_text, pattern); // Perform pattern matching
            auto stop = high_resolution_clock::now(); // Stop timer

            // Calculate duration in milliseconds
            double duration = duration_cast<microseconds>(stop - start).count() / 1000.0;

            // Log results to CSV file
            log_file << pattern.length() << ","
                << filename << ","
                << "\"" << processed_text << "\","
                << "\"" << pattern << "\","
                << duration << "\n";

            // Print results to console
            cout << "Pattern (" << pattern.length() << " chars) in " << filename
                << " found " << matches << " times, Time: " << duration << " ms\n";
        }
    }

    log_file.close(); // Close log file
    return 0;
}