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
  int numNodes = 1;

  for (size_t i = 0; i< patterns.size(); i++){
      size_t curNode = 0;
      for (size_t j = 0; j< patterns[i].size(); j++){
          char curChar = patterns[i][j];

          if (i==0 && j==0){
              edges e;
              e.emplace(curChar, numNodes);
              t.push_back(e);
              numNodes++;
              curNode++;
          }

          else if (t[curNode-1].count(curChar))
              curNode = t[curNode][curChar];

          else{
              if (t.size()==curNode){
                  edges newNode;
                  t.push_back(newNode);
              }
              t[curNode].emplace(curChar, numNodes);
              numNodes++;
              //curNode = newNodeIndex;
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









trie build_trie2(vector<string> & patterns) {
  trie t;
  for (size_t i = 0; i< patterns.size(); i++){
      size_t curNode = 0;
      for (size_t j = 0; j< patterns[i].size(); j++){
          char curChar = patterns[i][j];

          if (i==0 && j==0){
              edges e;
              t.push_back(e);
              t[curNode].emplace(curChar, 1);
              curNode = 1;
          }

          else if (curNode>0 && t[curNode-1].count(curChar) && !t.empty() )
              curNode = t[curNode-1][curChar];
          else if (curNode == 0 && t[curNode].count(curChar) && !t.empty())
              curNode = t[curNode][curChar];

          else{
              edges newNode;
              t.push_back(newNode);
              int newNodeIndex = t.size();
              t[curNode].emplace(curChar, newNodeIndex);
              curNode = newNodeIndex;
          }
      }
  }
  return t;
}
