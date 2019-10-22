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

// Explanation of how the code works can be found in lecture notes from week 4 of the course.

// Alphabet size: ACGT$; 5 symbols
const int sigma = 5;
const int A = 1, C = 2, G = 3, T = 4, Dollar = 0;

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


// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
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
