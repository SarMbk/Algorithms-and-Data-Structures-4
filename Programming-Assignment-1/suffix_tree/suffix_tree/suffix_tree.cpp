#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;


struct edge {
    size_t start;
    size_t length;
    edge(size_t start, size_t length) : start(start), length(length) {}
    vector<edge> children;
};


// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.
void add(const string& text, const edge& e, vector<string>& result) {
    result.push_back(text.substr(e.start,e.length));
    for (auto & e : e.children) {
        add(text,e,result);
    }
}


vector<string> ComputeSuffixTreeEdges(const string& text) {

    vector<string> result;

    vector<edge> root_edges;
    for (size_t i = 0; i < text.size(); ++i) {

        auto edges = &root_edges;
        auto suffix_size = text.size() - i;
        auto j = i;

        for (;;) {
            auto root_edge = find_if(begin(*edges),end(*edges),[&text,j](const edge & e){
                return text[j] == text[e.start];
            });
            if (root_edge != end(*edges)) {

                size_t length;
                for (length = 0; length < std::min(root_edge->length,suffix_size) && text[j+length] == text[root_edge->start+length]; ++length);

                if (length < root_edge->length) {

                    edge e { root_edge->start + length, root_edge->length - length };
                    swap(e.children,root_edge->children);

                    root_edge->length = length;

                    root_edge->children.push_back(std::move(e));
                    root_edge->children.emplace_back(j+length,suffix_size-length);

                    break;

                } else {
                    j += length;
                    suffix_size -= length;
                    edges = &root_edge->children;
                }
            } else {
                edges->emplace_back(j,suffix_size);
                break;
            }
        }
    }

    for (auto & e : root_edges) {
        add(text,e,result);
    }

    return result;
}


int main() {
  string text;
  cin >> text;
  vector<string> edges = ComputeSuffixTreeEdges(text);
  for (int i = 0; i < edges.size(); ++i) {
    cout << edges[i] << endl;
  }
  return 0;
}


/*
struct Node{
    string txt;
    Node* A;
    Node* C;
    Node* G;
    Node* T;
    Node* dSign;
    Node* parent;
};


struct Node* newNode(string data){
    Node* node = new Node;

    node->txt = data;
    node->A = nullptr;
    node->C = nullptr;
    node->G = nullptr;
    node->T = nullptr;
    node->dSign=nullptr;
    node->parent = nullptr;

    return node;
}


bool noChildren(Node* root){
    return root->A==nullptr && root->C==nullptr && root->G==nullptr && root->T==nullptr && root->dSign==nullptr;
}


Node* construct_suffix_tree(string text){
    // first make root;
    Node* root = newNode("");
    int n = text.size();

    for (int i=n-1; i>=0; i--){
        Node* curNode = root;
        string suffix = text.substr(i);
        //if first node:
        if (i==n-1){
            Node* n = newNode(suffix);
            n->parent = root;
            root->dSign = n;
        }
        else if (curNode->A==nullptr && suffix[0]=='A'){
            Node* n = newNode(suffix);
            n->parent = curNode;
            curNode->A = n;
        }
        else if (curNode->C==nullptr && suffix[0]=='C'){
            Node* n = newNode(suffix);
            n->parent = curNode;
            curNode->C = n;
        }
        else if (curNode->G==nullptr && suffix[0]=='G'){
            Node* n = newNode(suffix);
            n->parent = curNode;
            curNode->G = n;
        }
        else if (curNode->T==nullptr && suffix[0]=='T'){
            Node* n = newNode(suffix);
            n->parent = curNode;
            curNode->T = n;
        }
        else{

        }
    }
    return root;
}


void treeWalk(Node* node){
    if (node!=nullptr){
        treeWalk(node->A);
        treeWalk(node->C);
        treeWalk(node->G);
        treeWalk(node->T);
        treeWalk(node->dSign);
        cout << node->txt << endl;
    }
}



int main(){
    string text = "ACGT$";
    Node* root = construct_suffix_tree(text);
    treeWalk(root);

}
*/
