#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;
struct Child {
    char name[16];
};
struct Sympathy {
    int from;
    int to;
};
class Relationship{
private:
    bool** adjacency_matrix;
    int numberOfChildren;
    Child* children;
public:
    Relationship(int numberOfChildren, int amountOfSympathies, Child* children, Sympathy* sympathies){
        this->numberOfChildren = numberOfChildren;
        this->children = children;
        adjacency_matrix = new bool*[numberOfChildren];
        for(int column = 0; column < numberOfChildren; column++){
            adjacency_matrix[column] = new bool[numberOfChildren];
            for (int row = 0; row < numberOfChildren; row++) {
                adjacency_matrix[column][row] = false;
            }
        }
        for (int i = 0; i < amountOfSympathies; i++){
            adjacency_matrix[sympathies[i].to][sympathies[i].from] = true;
        }
    }
    vector<string> getUnloved(){
        vector<string> names;
        for(int column = 0; column < numberOfChildren; column++){
            bool unLoved = true;
            for (int row = 0; row < numberOfChildren; row++) {
                if (adjacency_matrix[column][row]){
                    unLoved = false;
                    break;
                }
            }
            if (unLoved)
                names.push_back((string)children[column].name);
        }
        return names;
    }
    vector<string> getUnhappy(){
        vector<string> names;
        for(int column = 0; column < numberOfChildren; column++){
            int row = column;
            bool reciprocity = true;
            bool feeling = false;
            for (row; row < numberOfChildren; row++) {
                if (!feeling){
                    if (adjacency_matrix[row][column]) feeling = true;
                }
                if (feeling) {
                    if (adjacency_matrix[row][column] && adjacency_matrix[column][row]) {
                        reciprocity = false;
                        break;
                    }
                }
            }
            if (reciprocity && feeling)
                names.push_back((string)children[column].name);
        }
        return names;
    }
    vector<string> getPets(){
        vector<string> names;
        int maxNumberOfFansByPerson = 0;
        int* childPerNumberOfFans = new int[numberOfChildren];
        for(int column = 0; column < numberOfChildren; column++){
            int fansOfChild = 0;
            bool unLoved = true;
            for (int row = 0; row < numberOfChildren; row++) {
                if (adjacency_matrix[column][row]){
                    fansOfChild++;
                }
            }
            if (fansOfChild > maxNumberOfFansByPerson) maxNumberOfFansByPerson = fansOfChild;
            childPerNumberOfFans[column] = fansOfChild;
        }
        for (int i = 0; i < numberOfChildren; i++) {
            if (childPerNumberOfFans[i] == maxNumberOfFansByPerson) {
                names.push_back((string)children[i].name);
            }
        }
        return names;
    }
    ~Relationship(){
        for (int i = 0; i < numberOfChildren; ++i) {
            delete[] adjacency_matrix[i];
        }
        delete[] adjacency_matrix, children;
    }
};
int getChildrenFromFile(string fileName, Child* children){
    ifstream file (fileName, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        int numberOfChildren = (int)file.tellg() / sizeof(Child);
        file.seekg (0, ios::beg);
        file.read((char*)children, numberOfChildren * sizeof(Child));
        file.close();
        return numberOfChildren;
    }
}
int getSympathiesFromFile(string fileName, Sympathy* sympathies){
    ifstream file (fileName, ios::in | ios::binary |ios::ate);
    if (file.is_open())
    {
        int amountOfSympathies = (int)file.tellg() / sizeof(Sympathy);
        file.seekg (0, ios::beg);
        file.read((char*)sympathies, amountOfSympathies * sizeof(Sympathy));
        file.close();
        return amountOfSympathies;
    }
}
void createFiles(string childrenFileName, string sympathiesFileName){
    Child* children = new Child[5];
    strcpy(children[0].name, "Alex");
    strcpy(children[1].name, "Bob");
    strcpy(children[2].name, "Kate");
    strcpy(children[3].name, "John");
    strcpy(children[4].name, "Alice");

    ofstream fileChildren (childrenFileName, ios::out | ios::binary);
    fileChildren.write((char*)children, 5 * sizeof(Child));
    fileChildren.close();
    delete[] children;

    Sympathy* sympathies = new Sympathy[7];
    sympathies[0].from = 0; sympathies[0].to = 4;
    sympathies[1].from = 1; sympathies[1].to = 2;
    sympathies[2].from = 1; sympathies[2].to = 4;
    sympathies[3].from = 2; sympathies[3].to = 0;
    sympathies[4].from = 2; sympathies[4].to = 1;
    sympathies[5].from = 3; sympathies[5].to = 2;
    sympathies[6].from = 3; sympathies[6].to = 4;

    ofstream fileSympathies (sympathiesFileName, ios::out | ios::binary);
    fileSympathies.write((char*)sympathies, 7 * sizeof(Sympathy));
    fileSympathies.close();
    delete[] sympathies;
}
void printAnswer(vector<string> names){
    for (vector<string>::iterator it = names.begin(); it != names.end() ; ++it)
        cout << *it << endl;
}
int getNumberOfChildren(string childrenFileName){
    ifstream file (childrenFileName, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        int numberOfChildren = (int)file.tellg() / sizeof(Child);
        file.close();
        return numberOfChildren;
    }
}
int getNumberOfSympathies(string sympathiesFileName){
    ifstream file (sympathiesFileName, ios::in | ios::binary |ios::ate);
    if (file.is_open())
    {
        int amountOfSympathies = (int)file.tellg() / sizeof(Sympathy);
        file.close();
        return amountOfSympathies;
    }
}

int main (int argc , char *argv[]){
    if (argc < 3) {
        std::cerr << "Error: missing file operands (children and sympathies filename)" << std::endl;
        return 1;
    }
    string childrenFileName = argv[1];
    string sympathiesFileName = argv[2];

    int action;
    Child* children;
    Sympathy* sympathies;
    int numberOfChildren, amountOfSympathies;

    createFiles(childrenFileName, sympathiesFileName);
    numberOfChildren = getNumberOfChildren(childrenFileName);
    amountOfSympathies = getNumberOfSympathies(sympathiesFileName);

    children = new Child[numberOfChildren];
    sympathies = new Sympathy[amountOfSympathies];
    getChildrenFromFile(childrenFileName, children);
    getSympathiesFromFile(sympathiesFileName, sympathies);

    Relationship relationship(numberOfChildren, amountOfSympathies, children, sympathies);

    cout << "Show list of.." << endl;
    cout << "1 - unloved children" << endl;
    cout << "2 - unhappy children" << endl;
    cout << "3 - pets children" << endl;
    cout << "other key - exit" << endl;

    while (true) {
        cin >> action;
        vector<string> names;
        switch (action) {
            case 0:
                delete[] children, sympathies;
                return 0;
            case 1:
                printAnswer(relationship.getUnloved());
                break;
            case 2:
                printAnswer(relationship.getUnhappy());
                break;
            case 3:
                printAnswer(relationship.getPets());
                break;
            default:
                delete[] children, sympathies;
                return 0;
        }
    }
}
