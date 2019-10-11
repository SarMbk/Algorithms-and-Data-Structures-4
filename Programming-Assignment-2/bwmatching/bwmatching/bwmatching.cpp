#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::istringstream;
using std::map;
using std::string;
using std::vector;
using namespace std;



// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position 
//       of this character in the sorted array of 
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt, map<char, int>& starts, map<char, vector<int> >& counts) {
  int textLen = bwt.length();

  // Populate the counts table
  for (int i=0; i<textLen; ++i){
      char symbol = bwt[i];

      if (!counts.count(symbol)){
          vector<int> symbolCounts (textLen, 0);
          counts.emplace(symbol, symbolCounts);
          counts[symbol][i]++;
          for (int j=i; j<textLen; ++j)
              counts[symbol][j] = counts[symbol][i];
      }

      else if (counts.count(symbol)){
          counts[symbol][i] = counts[symbol][i-1]+1;
          for (int j=i; j<textLen; ++j)
              counts[symbol][j] = counts[symbol][i];

      }
  }

  //Populate the starts map
  string text = bwt;
  sort(text.begin(), text.end());
  for (int i=0; i<textLen; ++i){
      char symbol = text[i];
      if (!starts.count(symbol)){
          starts.emplace(symbol, i);
      }
  }

}








// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern, 
                     const string& bwt, 
                     const map<char, int>& starts, 
                     const map<char, vector<int> >& counts) {
  // Implement this function yourself
  return 0;
}










int main(){
    string bwt = "smnpbnnaaaaa$a";
    map<char,int> starts;
    map<char, vector<int> > counts;
    PreprocessBWT(bwt,starts,counts);
    return 0;
}






















//int main() {
//  string bwt;
//  cin >> bwt;
//  int pattern_count;
//  cin >> pattern_count;

//  // Start of each character in the sorted list of characters of bwt,
//  // see the description in the comment about function PreprocessBWT
//  map<char, int> starts;

//  // Occurrence counts for each character and each position in bwt,
//  // see the description in the comment about function PreprocessBWT
//  map<char, vector<int> > counts;

//  // Preprocess the BWT once to get starts and occ_count_before.
//  // For each pattern, we will then use these precomputed values and
//  // spend only O(|pattern|) to find all occurrences of the pattern
//  // in the text instead of O(|pattern| + |text|).
//  PreprocessBWT(bwt, starts, counts);
//  for (int pi = 0; pi < pattern_count; ++pi) {
//    string pattern;
//    cin >> pattern;
//    int occ_count = CountOccurrences(pattern, bwt, starts, counts);
//    printf("%d ", occ_count);
//  }
//  printf("\n");
//  return 0;
//}
