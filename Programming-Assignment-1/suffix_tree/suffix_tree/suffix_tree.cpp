#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



// Attempt to solve per pseudocode in textbook pp 165 - 167
// Modified suffix trie is built. What is left is to create a suffix tree from a trie

typedef map<char, int> edges;
typedef vector<edges> trie;
typedef vector<pair<int, edges> > suffixTrie;


suffixTrie ModifiedSuffixTrieConstruction(string &text){
    suffixTrie t;
    pair<int, edges> e;
    t.push_back(e);
    int index = 1;

    int n = text.size();

    for (int i=0; i<n; ++i){
        int curNode = 0;
        for (int j=i; j<n; ++j){
            char curSymbol = text[j];

            if ( t[curNode].second.count(curSymbol) ){
                curNode = t[curNode].second[curSymbol];
            }
            else {
                t[curNode].first = -1;
                t[curNode].second[curSymbol] = index;
                pair<int, edges> newEdge;
                t.push_back(newEdge);
                curNode = index;
                index++;

            }
        }
        if(t[curNode].second.empty()){
            t[curNode].first = i;
        }
    }
    return t;
}


// This function is not yet finished
void SuffixTrieConstruction(string &text){
    suffixTrie t = ModifiedSuffixTrieConstruction(text);
    vector<string> result;
    for (int i=0; i<t.size(); ++i){
        string branch;
        if (t[i].second.size()==1){
        }
    }
}





int main() {
    string text;
    cin >> text;
    vector<string> edges = SuffixTree(text);
    for (int i = 0; i < edges.size(); ++i) {
        cout << edges[i] << endl;
    }
    return 0;
}




