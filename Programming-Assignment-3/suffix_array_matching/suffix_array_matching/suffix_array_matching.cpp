// Explanation of how the code works can be found in lecture notes from week 4 of the course.
// All methods to build suffix array are taken from problem set 3 problem 2.
// Method FindOccurrences2 works but does not pass the speed test. it is O(|pattern|*log|text|)
// Method FindOccurrences is buggy. Gives wrong results. It is implemented per Crochemore et al ch 4.
// That book is really dense and hard to follow.
// It is supposed to work in O(|pattern| + log|text|) the variable names they use are also terrible.
// Im giving up on this problem.

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


// FUNCTION PROTOTYPES FOR BUILDING SUFFIX ARRAY
int chToInd(char ch);
vector<int> SortCharacters(const string &text);
vector<int> ComputeCharClasses(const string &text, vector<int> &order);
vector<int> SortDoubled(const string &text, int L, vector<int> &order, vector<int> &classVec);
vector<int> UpdateClasses(vector<int> &newOrder, vector<int> &classVec, int L);
vector<int> BuildSuffixArray(const string& text);



// FUNCTION PROTORYPES FOR FINDING PATTERNS IN TEXT USING LCP ARRAYS
int LCPofSuffixes(const string &text, int i, int j, int equal);
vector<int> InvertSuffixArray(const vector<int>& order);
vector<int> InvertSuffixArray(const vector<int>& order);
vector<int> ComputeLCPArray(const string &text, const vector<int> &order);
int LCP(int d, int f, vector<int> &lcpArray, const string &text);
int getLenofLCP(const string &pattern, const string &text, int i, int l, const vector<int> &suffix_array);
pair<int,int> Search(   const string& text, const vector<int> &suffix_array,
                        vector<int> &lcpArray, const string &pattern);
vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array);


// FUNCTION DEFINITIONS FOR FINDING OCCURRENCES
// This algorithm is constructed based on pseudocode on p135 of course textbook Bioinformatics Algorithms vol 2 2nd ed.
// No algorithm using LCP array is used.
// This algorithm works but it's too slow
// Need to speed it ups with LCP
// Algorithm described in Algorithms on Strings, Crochemore et al ch 4
vector<int> FindOccurrences2(const string& pattern, const string& text, const vector<int>& suffix_array) {
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


// FUNCTION DEFINITIONS FOR FINDING PATTERNS IN TEXT USING LCP ARRAYS
// PER CROCHEMORE ET AL CH 4.
// CODE IS BUGGY, DOESN'T PRODUCE CORRECT RESULTS
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


int LCP(int d, int f, vector<int> &lcpArray, const string &text){
    int n = text.size();
    if (d+1 == f) return lcpArray[f];
    else{
        int i = (d+f)/2;
        int ans = min(LCP(d,i,lcpArray,text) , LCP(i,f,lcpArray,text));
        return ans;
    }
}


int getLenofLCP(const string &pattern, const string &text, int i, int l, const vector<int> &suffix_array){
    string subPattern(pattern.begin()+l, pattern.end());

    int ans=0;
    int index = l;
    while(index < pattern.size() && index < text.size()){
        if ( pattern[index] == text[suffix_array[i] + index]){
            ans++;
            index++;
        }
        else break;
    }
    return  ans;
}


pair<int,int> Search(const string& text, const vector<int> &suffix_array, vector<int> &lcpArray, const string &pattern){
    int n = text.size();
    int m = pattern.size();
    int d = -1, ld = 0, f = n, lf = 0;

    while (d+1 < f){
        int i = (d+f)/2;
        if (ld <= LCP(i,f,lcpArray,text) && LCP(i,f,lcpArray, text) < lf){
            d = i;
            ld = LCP(i,f,lcpArray,text);
        }
        else if (ld <= lf && lf < LCP(i,f,lcpArray,text)){
            f = i;
        }
        else if (lf <= LCP(d,i,lcpArray,text) && LCP(d,i,lcpArray,text) <ld ){
            f = i;
            lf = LCP(d,i,lcpArray,text);
        }
        else if (lf <= ld && ld < LCP(d,i,lcpArray,text) ){
            d = i;
        }
        else {
            int l = max(ld, lf);
            l+=getLenofLCP(pattern, text, i, l, suffix_array);
            if(l==m){
                int e = i;
                while (d+1 < e){
                    int j = (d+e)/2;
                    if (LCP(j,e,lcpArray,text) < m){
                        d = j;
                    }
                    else {
                        e = j;
                    }
                }
                if (LCP(d,e,lcpArray,text) >= m){
                    d = max(d-1, -1);
                }
                e = i;

                while (e+1 < f){
                    int j = (e+f)/2;
                    if (LCP(e,j,lcpArray,text) < m){
                        f = j;
                    }
                    else {
                        e = j;
                    }
                }

                if (LCP(e,f, lcpArray, text) >= m){
                    f = min(f+1, n);
                }
                pair<int, int> ans(d,f);
                return ans;
            }

            else if (l == n - suffix_array[i] || (l!=m && text[suffix_array[i]+l] < pattern[l]) ){
                d = i;
                ld = l;
            }
            else{
                f = i;
                lf = l;
            }
        }
    }
    pair<int,int> ans(d,f);
    return ans;
}


vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array){
    vector<int> lcpArray = ComputeLCPArray(text, suffix_array);
    pair<int,int> ans = Search(text,suffix_array,lcpArray,pattern);
    if (ans.first==-1) ans.first = 0;
    if (ans.second > text.size()-1){
        ans.second = text.size()-1;
    }
    vector<int> result(suffix_array.begin()+ans.first, suffix_array.begin()+ans.second);
    return result;
}



