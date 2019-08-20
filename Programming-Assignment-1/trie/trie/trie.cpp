#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;
using namespace std;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t;
  edges e;
  t.push_back(e);
  int index = 1;

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

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
