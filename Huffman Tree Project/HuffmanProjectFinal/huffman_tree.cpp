#include "huffman_tree.h"
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include<map>
#include <sstream>
#include <vector>

//Default constructor and destructor
Node::Node(){}
Node::~Node(){}

//Recursive function that adds the binary path to a map or array of strings (Both work but I went with the array
//because of O(1) access).
void huffman_tree::storingCodes(Node* node, std::string str) {
    if (node == nullptr) {
        return;
    }
    if (node->leaf){
        codes[(int)node->letter] = str;
        //paths.emplace(node->letter, str);
    }
    storingCodes(node->left, str + "0");
    storingCodes(node->right, str + "1");
}

//Primary function the build the huffman tree using a priority queue
huffman_tree::huffman_tree(const std::string &file_name){
    std::ifstream file(file_name);
    if(file.is_open()){

        //This method of reading a file into a string was mainly pulled from stack overflow (reused in the encoding function)
        std::string text = "";
        file.seekg(0, std::ios::end);
        file.seekg(0, std::ios::beg);
        text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::priority_queue<Node*, std::vector<Node*>, comparator> pq;
        int arr[128] = {0};
        for (unsigned int i = 0; i < text.length(); i++) {
            int ascii = (int)text.at(i);
            arr[ascii]++;
        }
        for (unsigned int i = 0; i < 128; i++) {
            if (arr[i] != 0) {
                Node *temp = new Node;
                temp->freq = arr[i];
                temp->letter = (char)i;
                temp->leaf = true;
                pq.push(temp);
            }
        }
        while (!pq.empty()) {
            if (pq.size() > 1) {
                Node *hold1 = pq.top();
                pq.pop();
                Node *hold2 = pq.top();
                pq.pop();

                Node *nonLeaf = new Node;
                nonLeaf->freq = hold1->freq + hold2->freq;
                nonLeaf->leaf = false;
                nonLeaf->left = hold1;
                nonLeaf->right = hold2;
                pq.push(nonLeaf);
            }
            if (pq.size() == 1) {
                root = pq.top();
                pq.pop();
            }
        }
        if(text.length() == 1){
            storingCodes(root,"0");
        }
        else storingCodes(root, "");
        file.close();
    }
}

//Default destructor
huffman_tree::~huffman_tree(){}

//Retrieves the binary path of the requested character using an array of strings or map
std::string huffman_tree::get_character_code(char character) const {
    return codes[(int)character];

    //If the map was used instead this for loop would find character
    /*
    for (auto it = paths.begin(); it != paths.end(); it++) {
        if (it->first == character) {
            return it->second;
        }
    }
    return "";
    */
}

//A function that encodes a file into binary paths as long as the character needed exists in the Huffman Tree
std::string huffman_tree::encode(const std::string &file_name) const {
    std::ifstream file(file_name);
    if(file.is_open()) {
        std::string text = "";
        file.seekg(0, std::ios::end);
        file.seekg(0, std::ios::beg);
        text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string encodedMessage = "";
        std::string binaryPath = "";
        for (unsigned int i = 0; i < text.length(); i++) {
            binaryPath = get_character_code(text[i]);
            if(binaryPath == ""){
                return "";
            }
            encodedMessage += binaryPath;
        }
        file.close();
        return encodedMessage;
    }
    return "";
}

//A function that decodes a string from the binary path using the Huffman Tree
std::string huffman_tree::decode(const std::string &string_to_decode) const {
    for(unsigned int i = 0; i < string_to_decode.length(); i++){
        if((int)string_to_decode.at(i) != 48 && (int)string_to_decode.at(i) != 49){
            return "";
        }
    }
    if(string_to_decode.length() == 1){
        std::string temp = "";
        temp += root->letter;
        return temp;
    }

    std::string str = "";
    Node* temp = root;
    for(unsigned int i = 0; i < string_to_decode.length(); i++){
        int value = (int)string_to_decode.at(i);
        if(value == 48){
            temp = temp->left;
        }
        else if(value == 49){
            temp = temp->right;
        }
        if(temp->leaf){
            str += temp->letter;
            temp = root;
        }
    }
    return str;
}