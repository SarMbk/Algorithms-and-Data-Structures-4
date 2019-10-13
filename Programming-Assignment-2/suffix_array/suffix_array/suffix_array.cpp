#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;
using namespace std;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string &text){
    vector<int> result;

    // vector will hold suffixes and their corresponding indices.
    // suffixes[i].first - holds the actual suffix string
    // suffixes[i].second - holds the position of the suffix
    vector<pair<string, int> > suffixes;

    // Populate vector of all suffixes and their corresponding positions
    for (int i = 0; i<text.size(); ++i){
        pair<string, int> suffix;
        suffix.first = text.substr(i);
        suffix.second = i;
        suffixes.push_back(suffix);
    }

    // Sort all suffixes lexicographically, by default it will sort pairs by first
    sort(suffixes.begin(), suffixes.end());

    // Copy all positions of suffixes into the result vector
    for (int i=0; i<suffixes.size(); ++i)
        result.push_back(suffixes[i].second);

    return result;
}



int main() {
  string text;
  cin >> text;
  vector<int> suffix_array = BuildSuffixArray(text);
  for (int i = 0; i < suffix_array.size(); ++i) {
    cout << suffix_array[i] << ' ';
  }
  cout << endl;
  return 0;
}
