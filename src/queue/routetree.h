#ifndef ROUTE_TREE_H
#define ROUTE_TREE_H

#include<string>
#include<vector>
#include "../utils/stringutils.h"

template<typename T>
class RouteTree {

private:
    
    struct Node {
        std::string key;
        bool isUseful;
        T t;
        std::vector<Node *> childVec;
    };

    Node *root;

    static const char DELIMITER;
    static const std::string WILDCARD;

    void deleteAll(Node *node);

public:

    RouteTree();
    ~RouteTree();

    void add(const std::string &str, const T &t);
    T get(const std::string &str, const T &defaultValue);

};

template<typename T>
const char RouteTree<T>::DELIMITER = '/';
template<typename T>
const std::string RouteTree<T>::WILDCARD = "*";

template<typename T>
RouteTree<T>::RouteTree(){
    root = new Node {
        .key = "/",
        .isUseful = false
        };
}

template<typename T>
RouteTree<T>::~RouteTree(){
    deleteAll(root);
}

template<typename T>
void RouteTree<T>::deleteAll(Node *node){
    for(auto child : node -> childVec){
        deleteAll(child);
    }
    delete node;
}

template<typename T>
void RouteTree<T>::add(const std::string &str,  const T &t){
    std::vector<std::string> strVec_temp = split(str, DELIMITER);
    std::vector<std::string> strVec;
    for(auto &s : strVec_temp){
        if(!s.empty()) strVec.push_back(s);
    }
    size_t size = strVec.size();
    Node *temp = root;
    for(size_t i = 0; i < size; i++){
        std::string s = strVec[i];
        bool isMatch = false;
        for(auto child : temp -> childVec){
            if(s == child -> key){
                temp = child;
                isMatch = true;
                break;
            }
        }
        if(isMatch) continue;
        Node *newNode = new Node{.key = s, .isUseful = false};
        temp -> childVec.push_back(newNode);
        temp = newNode;
    }
    if(temp != root){
        temp -> isUseful = true;
        temp -> t = t;
    }
}

template<typename T>
T RouteTree<T>::get(const std::string &str, const T &defaultValue){
    std::vector<std::string> strVec_temp = split(str, DELIMITER);
    std::vector<std::string> strVec;
    for(auto &s : strVec_temp){
        if(!s.empty()) strVec.push_back(s);
    }
    size_t size = strVec.size();
    Node *temp = root;
    std::vector<T> resultVec;
    for(size_t i = 0; i < size; i++){
        std::string s = strVec[i];
        for(auto child : temp -> childVec){
            if(child -> key == WILDCARD && child -> isUseful){
                resultVec.push_back(child -> t);
                break;
            }
        }
        bool isMatch = false;
        for(auto child : temp -> childVec){
            if(s == child -> key){
                if(i == size - 1 && child -> isUseful){
                    resultVec.push_back(child -> t);
                }
                temp = child;
                isMatch = true;
                break;
            }
        }
        if(!isMatch) break;
    }
    if(resultVec.empty()) return defaultValue;
    return resultVec[resultVec.size() - 1];
}


#endif