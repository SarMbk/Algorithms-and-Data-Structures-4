#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


typedef map<char, size_t> edges;
typedef vector<edges> trie;


// Function to build a trie from a vector of patterns
trie build_trie(const vector<string> & patterns) {
  trie t;
  edges e;
  t.push_back(e);
  size_t index = 1;

  for (size_t i = 0; i< patterns.size(); i++){
      size_t curNode = 0;
      for (size_t j = 0; j< patterns[i].size(); j++){
          char letter = patterns[i][j];

          if (t[curNode].count(letter)){
              curNode = t[curNode][letter];
          }
          else{
              t[curNode][letter] = index;
              edges e;
              t.push_back(e);
              curNode = index;
              index++;
          }
      }
  }
  return t;
}


// Algorithm implemented based on problem handout.
void PrefixTrieMatch (const string& text, size_t n, vector<size_t> &result, trie &t){
    size_t curNode = 0;
    size_t curIndex = n;

    while (true){
        char curSymbol = text[curIndex];

        if (t[curNode].empty()){
            result.push_back(n);
            break;
        }
        else if (t[curNode].count(curSymbol)){
            curIndex++;
            curNode = t[curNode][curSymbol];
        }
        else break;
    }

    return;
}


vector<size_t> TrieMatch(const string& text, const vector<string> &patterns){
    vector<size_t> result;
    trie t = build_trie(patterns);
    for (size_t i=0; i<text.size(); i++){
        PrefixTrieMatch(text, i, result, t);
    }
    return result;
}


int main (){
    string txt;
    cin >> txt;

    size_t n;
	cin >> n;

	vector <string> patterns (n);
    for (size_t i = 0; i < n; i++)
		cin >> patterns[i];

    vector <size_t> ans;
    ans = TrieMatch(txt, patterns);

    for (size_t i = 0; i < ans.size (); i++){
		cout << ans[i];
        if (i + 1 < ans.size ())
			cout << " ";
		else
			cout << endl;
	}

	return 0;
}
