#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// Rotates text towards left by numPos. Returns copy of rotated string.
string leftrotate(const string &text, int numPos) {
    string ans = text;
    reverse(ans.begin(), ans.begin()+numPos);
    reverse(ans.begin()+numPos, ans.end());
    reverse(ans.begin(), ans.end());
    return ans;
}

// Rotates text towards right by numPos. Returns copy of rotated string.
string rightrotate(const string &text, int numPos) {
    return leftrotate(text, text.length()-numPos);
}

// Makes all rotations of text and pushes them into rotations vector
void makeBWTvector(vector<string> &rotations, const string &text){
    for (int i=0; i<text.size(); ++i){
        string s = rightrotate(text, i);
        rotations.push_back(s);
    }
    sort(rotations.begin(), rotations.end());
}

// Returns a Burrows Wheeler transform of text.
string BWT(const string& text) {
  string result = "";
  vector<string> rotations;
  makeBWTvector(rotations, text);

  int lastChar = text.size()-1;
  for (int i=0; i<rotations.size(); ++i)
      result+=rotations[i][lastChar];

  return result;
}



string InverseBWT(const string& bwt) {
  string text = "";

  // write your code here

  return text;
}

int main() {
  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
