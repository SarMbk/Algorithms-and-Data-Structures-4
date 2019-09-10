#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;


// Tree representation; E.second = true if current character is at the end of pattern; false otherwise
typedef pair <size_t, bool > E;
typedef map< char, E > edges;
typedef vector<edges> trie;


// Function to build a trie from a vector of patterns
trie build_trie_extended(const vector<string> & patterns) {
  trie t;
  edges e;
  t.push_back(e);
  size_t index = 1;

  for (size_t i = 0; i< patterns.size(); i++){
      size_t curNode = 0;
      for (size_t j = 0; j< patterns[i].size(); j++){
          char letter = patterns[i][j];
          size_t ptnLength = patterns[i].size();

          // If there is a letter in current tree
          if (t[curNode].count(letter)){
              // place the appropriate flag if we are at the end of the current pattern
              if (t[curNode][letter].second != true){
                  t[curNode][letter].second = (j==ptnLength-1 ? true : false) ;
              }
              curNode = t[curNode][letter].first;
          }

          // If there is no such letter in the current tree
          else if (!t[curNode].count(letter)){
              t[curNode][letter].first = index;
              // Place the appropriate flag if we are at the end of the current pattern
              if (t[curNode][letter].second != true){
                  t[curNode][letter].second = (j==ptnLength-1 ? true : false);
              }
              edges e;
              t.push_back(e);
              curNode = index;
              index++;
          }
      }
  }
  return t;
}


// Algorithm implemented for problem 2 based on problem handout, and extended for problem 3
void PrefixTrieMatch_Extended (const string& text, size_t i, vector<size_t> &result, trie &t, unordered_map<size_t, int> & recorded){
    size_t curNode = 0;
    size_t curIndex = i;

    while (true){
        char curSymbol = text[curIndex];

        if (t[curNode].empty())
            break;

        else if (t[curNode].count(curSymbol)){
            // If the trie indicates that current symbol is at the end of a pattern
            if (t[curNode][curSymbol].second == true){
                if (!recorded.count(i)){        //here we use hashmap to avoid pushing duplicate entries into the result
                    recorded.emplace(i,1);
                    result.push_back(i);
                }
            }
            curIndex++;
            curNode = t[curNode][curSymbol].first;
        }
        else break;
    }


    return;
}


vector<size_t> TrieMatch_Extended(const string& text, const vector<string> &patterns){
    vector<size_t> result;
    trie t = build_trie_extended(patterns);
    unordered_map<size_t, int> recorded;        // the hashmap is used to track duplicate entries
    for (size_t i=0; i<text.size(); i++){
        PrefixTrieMatch_Extended(text, i, result, t, recorded);
    }
    recorded.clear();
    return result;
}


int main (void){
    string txt;
    cin >> txt;

    size_t n;
	cin >> n;

	vector <string> patterns (n);
    for (size_t i = 0; i < n; i++){
		cin >> patterns[i];
	}

    vector <size_t> ans;
    ans = TrieMatch_Extended(txt, patterns);

    for (size_t i = 0; i < ans.size (); i++){
        cout << ans[i];
        if (i + 1 < ans.size ())
            cout << " ";
        else
            cout << endl;
    }

	return 0;
}
