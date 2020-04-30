#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <bitset>

struct hashNode{
    int key;
    int value;
    hashNode *next;

    hashNode(int key, int value){
        this->key = key;
        this->value = value;
        this->next = NULL;
    }
};

struct hashTable{
    int totalCapacity;
    double currentCapacity;
    int collisions;
    hashNode **arr;

    hashTable(int tableSize){
        this->totalCapacity = tableSize;
        this->currentCapacity = 0.0;
        this->collisions = 0;
        this->arr = new hashNode*[tableSize];

        for(int i = 0; i < tableSize; i++){
            arr[i] = NULL;
        }
    }

    //Key Mod
//    int hashCode(int key){
//        return key%totalCapacity;
//    }

    //Mid-Square
    int hashCode(int key){
        return std::bitset<8>(std::bitset<19>(key*key).to_string().substr(5, 8)).to_ulong();
    }

    double loadFactor(){
        return currentCapacity/totalCapacity;
    }

    void place(int key, int value){
        hashNode *Value = new hashNode(key, value);
        int Index = hashCode(key);
        bool exists = true;

        if(Index > totalCapacity - 1) {
            Index %= totalCapacity;
        }
        if(arr[Index] != NULL && arr[Index]->key != key){
            collisions++;
            hashNode *temp = arr[Index];
            while(temp->next){
                if(temp->next->key == key){
                    collisions--;
                    exists = false;
                }
                temp = temp->next;
            }
            if(exists){
                temp->next = Value;
                currentCapacity++;
            }
        }
        else if(arr[Index] == NULL){
            arr[Index] = Value;
            currentCapacity++;
        }
    }

    void printTable(){
        for(int i = 0; i < totalCapacity; i++){
            if(arr[i] == NULL){
                std::cout << i << std::endl;
                continue;
            }
            hashNode *temp = arr[i];
            std::cout << i;
            while(temp){
                std::cout << " (" << temp->key << ", " << temp->value << ")";
                temp = temp->next;
            }
            std::cout << std::endl;
        }
    }
};

void runTuable(int size){
    hashTable *table = new hashTable(size);

    std::ofstream file ("hash.csv");
    file << "Item Number" << "," << "Load Factor" << "," << "Collisions" << std::endl;
    int i = 0;
    double capTemp = 0.0;

    while(table->loadFactor() != 1.0){
        int random = rand()%((size*3)+1);
        table->place(random,1);
        if(table->currentCapacity > capTemp) {
            std::cout << table->collisions << ", " << table->loadFactor() << std::endl;
            file << i + 1 << "," << table->loadFactor() << "," << table->collisions << "\n";
            i++;
        }
        capTemp = table->currentCapacity;
    }
    file.close();
    table->printTable();
}

int main() {
    runTuable(250);
    return 0;
}