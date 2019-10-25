// Explanation of how the code works can be found in lecture notes from week 4 of the course.
// All methods to build suffix array are taken from problem set 3 problem 2.


#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

// CONSTANTS
const int sigma = 5;
const int A = 1, C = 2, G = 3, T = 4, Dollar = 0; // Alphabet size: ACGT$; 5 symbols

// FUNCTION PROTPTYPES FOR BUILDING SUFFIX ARRAY
int chToInd(char ch);
vector<int> SortCharacters(const string &text);
vector<int> ComputeCharClasses(const string &text, vector<int> &order);
vector<int> SortDoubled(const string &text, int L, vector<int> &order, vector<int> &classVec);
vector<int> UpdateClasses(vector<int> &newOrder, vector<int> &classVec, int L);
vector<int> BuildSuffixArray(const string& text);


// FUNCTION DEFINITIONS FOR FINDING OCCURRENCES
// This algorithm is constructed based on pseudocode on p135 of course textbook Bioinformatics Algorithms vol 2 2nd ed.
// No algorithm using LCP array is used.
// This algorithm works but it's too slow
// Need to speed it ups with LCP
// Algorithm described in Algorithms on Strings, Crochemore et al ch 4
vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array) {
  vector<int> result;

  int minIndex = 0;
  int maxIndex = text.size();

  while(minIndex < maxIndex){
      int midIndex = (minIndex + maxIndex)/2;
      int res = strncmp(pattern.c_str(), text.c_str() + suffix_array[midIndex], pattern.size());
      if (res > 0)
          minIndex++;
      else
          maxIndex = midIndex;
  }

  int first = minIndex;
  maxIndex = text.size();

  while(minIndex < maxIndex){
      int midIndex = (minIndex + maxIndex)/2;
      int res = strncmp(pattern.c_str(), text.c_str() + suffix_array[midIndex], pattern.size());
      if (res < 0)
          maxIndex = midIndex;
      else
          minIndex = midIndex + 1;
  }

  int last = maxIndex;

  while (first < last){
      result.push_back(suffix_array[first]);
      ++first;
  }

  return result;
}





// MAIN METHOD
int main() {
  char buffer[100001];
  scanf("%s", buffer);
  string text = buffer;
  text += '$';
  vector<int> suffix_array = BuildSuffixArray(text);
  int pattern_count;
  scanf("%d", &pattern_count);
  vector<bool> occurs(text.length(), false);
  for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
    scanf("%s", buffer);
    string pattern = buffer;
    vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
    for (int j = 0; j < occurrences.size(); ++j) {
      occurs[occurrences[j]] = true;
    }
  }
  for (int i = 0; i < occurs.size(); ++i) {
    if (occurs[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}


// FUNCTION DEFINITIONS FOR BUILDING SUFFIX ARRAY
int chToInd(char ch){
    if (ch=='A') return A;
    else if (ch=='C') return C;
    else if (ch=='G') return G;
    else if (ch=='T') return T;
    else if (ch=='$') return Dollar;
}

vector<int> SortCharacters(const string &text){
    vector<int> order(text.size(), 0);
    vector<int> count(sigma, 0);
    for (int i = 0; i < text.size(); i++){
        int Si = chToInd(text[i]);
        count[Si]++;
    }
    for (int j = 1; j<sigma; j++){
        count[j] = count[j] + count[j-1];
    }
    for(int i = text.size()-1; i>=0; i--){
        int c = chToInd(text[i]);
        count[c]--;
        order[count[c]] = i;
    }
    return order;
}

vector<int> ComputeCharClasses(const string &text, vector<int> &order){
    vector<int> classVec (text.size(), 0);
    classVec[order[0]] = 0;
    for (int i = 1; i < text.size(); i++){
        if (text[order[i]] != text[order[i-1]]){
            classVec[order[i]] = classVec[order[i-1]] + 1;
        }
        else{
            classVec[order[i]] = classVec[order[i-1]];
        }
    }
    return classVec;
}

vector<int> SortDoubled(const string &text, int L, vector<int> &order, vector<int> &classVec){
    int s = text.size();
    vector<int> count(s, 0);
    vector<int> newOrder(s, 0);

    for (int i=0; i<s; i++){
        count[classVec[i]]++;
    }

    for (int j = 1; j<s; j++){
        count[j] = count[j] + count[j-1];
    }

    for (int i = s-1; i>=0; i--){
        int start = (order[i] - L + s) % s;
        int cl = classVec[start];
        count[cl]--;
        newOrder[count[cl]] = start;
    }
    return newOrder;
}

vector<int> UpdateClasses(vector<int> &newOrder, vector<int> &classVec, int L){
    int n = newOrder.size();
    vector<int> newClass(n, 0);
    newClass[newOrder[0]] = 0;
    for (int i = 1; i<n; i++){
        int cur = newOrder[i], prev = newOrder[i-1];
        int mid = (cur+L), midPrev = (prev+L)%n;
        if (classVec[cur] != classVec[prev]  || classVec[mid]!=classVec[midPrev]){
            newClass[cur] = newClass[prev]+1;
        }
        else{
            newClass[cur] = newClass[prev];
        }
    }
    return newClass;
}

vector<int> BuildSuffixArray(const string& text) {
  vector<int> order = SortCharacters(text);
  vector<int> classVec = ComputeCharClasses(text, order);
  int L = 1;
  while (L < text.size()){
      order = SortDoubled(text,L, order, classVec);
      classVec = UpdateClasses(order, classVec, L);
      L = 2*L;
  }
  return order;
}



/*
int LCPofSuffixes(const string &text, int i, int j, int equal){
    int lcp = max(0, equal);
    while (i+lcp < text.size() && j+lcp < text.size()){
        if (text[i+lcp] == text[j+lcp])
            lcp++;
        else
            break;
    }
    return lcp;
}

vector<int> InvertSuffixArray(const vector<int>& order){
    int n = order.size();
    vector<int> pos(n, 0);
    for (int i=0; i < n; ++i){
        pos[order[i]] = i;
    }
    return pos;
}

vector<int> ComputeLCPArray(const string &text, const vector<int> &order){
    vector<int> lcpArray(text.size()-1,0);
    int lcp = 0;
    vector<int> posInOrder = InvertSuffixArray(order);
    int suffix = order[0];
    for (int i=0; i<text.size(); ++i){
        int orderIndex = posInOrder[suffix];
        if (orderIndex == text.size()-1){
            lcp = 0;
            suffix = (suffix+1)%text.size();
            continue;
        }
        int nextSuffix = order[orderIndex+1];
        lcp = LCPofSuffixes(text, suffix, nextSuffix, lcp-1);
        lcpArray[orderIndex] = lcp;
        suffix = (suffix+1)%text.size();
    }
    return lcpArray;
}
*/
