#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <windows.h>
using namespace std;

// constants
const string DATA_DIR = "datafiles/";
const string BASE_DATA_FILE_NAME = "Data";
const string DATA_FILE_EXT = ".txt";
#define MAX_CASES 27
#define MAX_ROUNDS 11
// global vars
int CASES[MAX_CASES]; // index = case, value = money in case
int ROUNDS[MAX_ROUNDS]; // index = round #, val = how many cases to choose in that round
int numCases = 0;
int numRounds = 0;

// function protos
string getDataString(int dataNum);
bool loadArray(int dataNum);
void initArray(int arr[], int size);
void printArray(int arr[], int size);
int generateRandom(int lower, int upper);
int printChooseNumCases();
bool isValidCases(int numCases);
void printMoneyInCases();

// main
int main() {
    cout << "Welcome to Deal or No Deal!" << endl;
    numCases = printChooseNumCases();
    loadArray(numCases);
//    printArray(CASES, MAX_CASES);
//    cout << endl;
//    printArray(ROUNDS, MAX_ROUNDS);
    printMoneyInCases();
    return 0;
}

// function decs
string getDataString(int dataNum){
    return DATA_DIR + BASE_DATA_FILE_NAME + to_string(dataNum) + DATA_FILE_EXT;
}

bool loadArray(int dataNum){
    numCases = dataNum;
    ifstream df;
    df.open(getDataString(dataNum));
    if (!df.is_open()){
        cout << "file could not be opened" << endl;
        return false;
    }
    initArray(CASES, MAX_CASES);
    int line;
    df >> line;
    int i = 1;
    // load cases
    while(i <= numCases){
        CASES[i++] = line;
        df >> line;
    }

    // load rounds
    initArray(ROUNDS, MAX_ROUNDS);
    numRounds = line;
    df >> line;
    i = 1;
    while(i <= numRounds){
        int val = line;
        ROUNDS[i++] = val;
        df >> line;
    }
    df.close();
}

void initArray(int arr[], int size){
    for (int i = 0; i < size; ++i) {
        arr[i] = -1;
    }
}

void printArray(int arr[], int size){
    for (int i = 0; i < size; ++i) {
        if (i % 5 == 0){
            cout << "\n";
        }
        cout << "\t" << arr[i];
    }
}

int generateRandom(int lower, int upper){
    //srand(time(0));
    int random = lower + rand() % (upper - lower + 1);
    return random;
}

// gui functions
int printChooseNumCases(){
    int val;
    cout << "How many cases do you want in the game (16, 18, 20, 22, 24 ,26)? ";
    cin >> val;
    while (!isValidCases(val)){
        cin >> val;
    }
    return val;
}

void printMoneyInCases(){
    int half = numCases/2;
    cout << "-------------------------" << endl;
    int i = 1;
    int k = half+1;
    while (i <= half){
        cout << "|\t" << CASES[i++] << "\t" << CASES[k++] << "\t|" << endl;
    }
    cout << "-------------------------" << endl;
}

bool isValidCases(int cases) {
    bool val = true;
    if (cases % 2 != 0){
        val = false;
    }
    if (cases < 16 || cases > 26){
        val = false;
    }

    if (!val){
        cout << "Invalid nubmer of cases chosen, try again (16, 18, 20, 22, 24 ,26) ";
    }

    return val;
}
