#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <map>
#include <string>

class Node{
public:
    char letter = '\0';
    int freq = 0;
    bool leaf = false;
    Node* left = nullptr;
    Node* right = nullptr;

    Node();
    ~Node();
};

//Comparator class that lets the priority queue sort by frequency
class comparator{
public:
    bool operator() (const Node* p1, const Node* p2){
        return p1->freq > p2->freq;
    }
};

class huffman_tree{
public:
    Node* root;
    std::string codes[128] = {};

    //This is the map of paths that could be used instead of the array of strings
    //std::map<char, std::string> paths;

    huffman_tree(const std::string &file_name);
    ~huffman_tree();

    std::string get_character_code(char character) const;
    std::string encode(const std::string &file_name) const;
    std::string decode(const std::string &string_to_decode) const;
    void storingCodes(Node*, std::string);
};

#endif