#include "../src/queue/routetree.h"
#include<iostream>
#include<string>

using namespace std;

int main(){

    RouteTree<string> tree;

    tree.add("/aa", "aa");
    tree.add("/bb", "bb");
    tree.add("/aa/bb", "aa/bb");
    tree.add("/bb/*", "bb/*");
    tree.add("bb/aa", "bb/aa");
    tree.add("bb/aa/*", "bb/aa/*");

    string s = "default";

    cout << tree.get("/cc", s) << endl;
    cout << tree.get("/aa/bb", s) << endl;
    cout << tree.get("/aa/", s) << endl;
    cout << tree.get("/bb/bb", s) << endl;
    cout << tree.get("bb/aa/", s) << endl;
    cout << tree.get("bb/aa/cc", s) << endl;
    cout << tree.get("bb", s) << endl;

}