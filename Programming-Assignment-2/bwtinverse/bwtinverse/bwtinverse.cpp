#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// To understand this code and problem read ch9 of the Bioinformatics Algorithms book (vol2 ed2)

// Functor to use when hashing pair to int
struct hash_pair {
    size_t operator()(const pair<char, int>& p) const{
        auto hash1 = hash<char>{}(p.first);
        auto hash2 = hash<int>{}(p.second);
        return hash1 ^ hash2;
    }
};


void populateMap (unordered_map<pair<char, int> , int, hash_pair > &firstToLast, const string &bwt){
    // Populate hashmap with number of occurences of each character and their position in the last column
    unordered_map<char, int> counts;
    for (int i=0; i<bwt.size(); ++i){
        char ch = bwt[i];

        if ( !counts.count(ch) ){
            counts.emplace(ch,1);
            pair<char, int> charAndNum (ch, 1);
            firstToLast.emplace(charAndNum, i);
        }

        else if ( counts.count(ch) ){
            counts[ch]++;
            pair<char, int> charAndNum(ch, counts[ch]);
            firstToLast.emplace(charAndNum, i);
        }
    }
}


void populateVector(vector< pair<char, int> > &firstColCounts,  const string &firstCol){
    unordered_map<char, int> counts;
    for (int i=0; i<firstCol.size(); ++i){
        char ch = firstCol[i];

        if (!counts.count(ch)){
            pair<char, int> charAndNum(ch, 1);
            counts.emplace(charAndNum);
            firstColCounts.push_back(charAndNum);
        }

        else if (counts.count(ch)){
            counts[ch]++;
            pair<char, int> charAndNum(ch, counts[ch]);
            firstColCounts.push_back(charAndNum);
        }
    }
}


string InverseBWT(const string& bwt) {
    string text = "";
    string firstCol = bwt;
    sort(firstCol.begin(), firstCol.end());

    // map of < <character -  instance number of character>  - character position>
    // e.g. if bwt string is "abab" the first to last map will be: [a-1 : 0, b-1 : 1, a-2 : 2, b-2 : 3]
    // it maps the character and instance pair to its position in the bwt string
    unordered_map<pair<char, int> , int, hash_pair > firstToLast;
    populateMap (firstToLast, bwt);

    // Populate vector firstColCounts. e.g. if first col string is "abcabc" then vector is [a-1, b-1, c-1, a-2, b-2, c-2]
    vector <pair<char, int> > firstColCounts;
    populateVector (firstColCounts, firstCol);

    // Use the first to last property to reconstruct the original string
    pair<char, int> chAndNum = firstColCounts[0];
    for (int i=0; i<firstColCounts.size(); ++i){
        //pair<char, int> chAndNum = firstColCounts[i];
        int nextCharPos = firstToLast.at(chAndNum);
        char nextChar = firstCol[nextCharPos];
        text.push_back(nextChar);
        chAndNum = firstColCounts[nextCharPos];
    }

  return text;
}


int main() {
  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
