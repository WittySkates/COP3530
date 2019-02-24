#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <math.h>

using namespace std;

/* Is the class that holds the information of each element that gets pushed into the linked list.
 * Value is the number provided by the user
 * rowPosition is is the Y location of the node
 * colPosition is the X value of the node
 */
struct Node{
    int value;
    int rowPosition;
    int colPosition;

    Node(int value, int row, int col){
        this->value = value;
        rowPosition = row;
        colPosition = col;
    }
};

//Prototypes for the functions used in this project
list<Node> findMinor(list<Node>, list<Node>::iterator atNode);
int determinantOfMatrix(list<Node> matrix, int rowAmount);
void readMatrix(list <Node> *matrix);

//Is a recursive function that determines the determinant of the matrix
int determinantOfMatrix(list<Node> matrix, int rowAmount){
    int determinant = 0;
    if(matrix.empty()){
        return 0;
    }
    //Once the matrix size reaches one the recursion will stop for that node
    if(matrix.size() == 1){
        return matrix.front().value;
    }
    for(int i = 0; i < rowAmount; i++){
        auto iter = matrix.begin();
        advance(iter, i);
        int matrixCoefficient = iter->value;
        determinant += (pow(-1, iter->colPosition)) * matrixCoefficient * determinantOfMatrix(findMinor(matrix, iter), rowAmount-1);
    }
    return determinant;
}

//This function finds the minor matrix at the given node and returns a new matrix
list<Node> findMinor(list<Node> matrix, list<Node>::iterator atNode){
    list<Node>::iterator iter;
    //Erases the nodes in the same column and row
    for(iter = matrix.begin(); iter != matrix.end();){
        if(iter->colPosition == atNode->colPosition || iter->rowPosition == atNode->rowPosition){
            iter = matrix.erase(iter);
        }
        else iter++;
    }
    /* Shift the reaming nodes up one. If any of the remaining
     * nodes were to the right of the initial node, shift them to the left
     */
    for(iter = matrix.begin(); iter != matrix.end();iter++){
        iter->rowPosition = iter->rowPosition-1;
        if(iter->colPosition > atNode->colPosition){
            iter->colPosition = iter->colPosition-1;
        }
    }
    return matrix;
}

//Reads the matrix, determines whether it is square, and pushes all the non-zero values into a linked list
void readMatrix(list <Node> *matrix){
    string str;
    int cols = 1;
    int colCheck;
    int rows = 0;
    bool first = true;

    while(getline(cin,str)) {
        if (str.empty()) {
            break;
        }
        /* Looks at the first line and determines the amount of columns there should be. This will be the base case for
         * testing the other rows and columns against
         */
        if (first){
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == ' ') {
                    cols++;
                }
            }
            first = false;
        }
        //Checks all the columns to make sure that they adhere to the base case above
        colCheck = 1;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == ' ') {
                colCheck++;
            }
        }
        if(colCheck != cols) {
            break;
        }
        stringstream individual(str);
        //Pushes all non-zero values into a linked list
        for(int i = 0; i < cols; i++){
            getline(individual,str,' ');
            if(stoi(str) != 0) {
                Node temp = Node(stoi(str), rows, i);
                matrix->push_back(temp);
            }
        }
        rows++;
    }
    //Checks all the rows to make sure that they adhere to the base case above
    if(rows != cols) {
        cout << "Error! Non-square matrix!" << endl;
        exit(0);
    }
}

//The main driver of the program
int main() {
    //Creates the initial matrix all the elements will be stored in
    list <Node> matrix;
    int rowAmount = 0;
    readMatrix(&matrix);
    /* Creates an iterator that determines the amount of Nodes in a row. This is because we didn't include the zeros so the
     * amount of elements in the row is different from provided matrix
     */
    list<Node>::iterator it;
    for(it = matrix.begin(); it != matrix.end(); it++){
        if(it->rowPosition == 0){
            rowAmount++;
        }
    }
    cout << determinantOfMatrix(matrix, rowAmount);
    return 0;
}