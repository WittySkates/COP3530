
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <bitset>

struct hashNode{
    int key;
    int value;

    hashNode(int key, int value){
        this->key = key;
        this->value = value;
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
        bool Collision = false;

        if(Index > totalCapacity - 1){
            Index %= totalCapacity;
        }
        while(arr[Index] != NULL && arr[Index]->key != key){
            if(!Collision){
                collisions++;
                Collision = true;
            }
            Index++;
            if(Index == totalCapacity){
                Index = 0;
            }
        }
        if(arr[Index] == NULL){
            currentCapacity++;
        }
        arr[Index] = Value;
    }

    void printTable(){
        for(int i = 0; i < totalCapacity; i++){
            if(arr[i] == NULL){
                std::cout << i << std::endl;
                continue;
            }
            std::cout << i << " (" << arr[i]->key << ", " << arr[i]->value << ")" << std::endl;
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
    runTuable(20);
    return 0;
}

