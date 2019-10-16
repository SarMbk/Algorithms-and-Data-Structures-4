#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;
using namespace std;


// This solution implemets Knuth Morris Pratt algorithm. See wk3 materials for explanation.


vector<int> ComputePrefixFunction(string & pattern){
    size_t p = pattern.size();
    vector<int> s(p, 0);
    int border = 0;
    for (int i=1; i<p; ++i){
        while( border>0 && pattern[i]!=pattern[border] )
            border = s[border-1];
        if (pattern[i] == pattern[border])
            border++;
        else
            border = 0;
        s[i] = border;
    }
    return s;
}


// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where 
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {
  vector<int> result;
  string PT = pattern + "$" + text;
  vector<int> s = ComputePrefixFunction(PT);
  int P = pattern.size();
  int S = PT.size();

  for (int i = P+1; i<S; ++i){
      if(s[i] == pattern.size())
          result.push_back(i - 2*P);
  }
  return result;
}


int main() {
  string pattern, text;
  cin >> pattern;
  cin >> text;
  vector<int> result = find_pattern(pattern, text);
  for (int i = 0; i < result.size(); ++i) {
    printf("%d ", result[i]);
  }
  printf("\n");
  return 0;
}
