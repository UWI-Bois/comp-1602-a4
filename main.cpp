#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <cstdlib>
using namespace std;

// constants
const string DATA_DIR = "datafiles/";
const string BASE_DATA_FILE_NAME = "Data";
const string DATA_FILE_EXT = ".txt";
#define MIN_CASES 16
#define MAX_CASES 26
#define MAX_CASES_SIZE 27
#define MAX_ROUNDS_SIZE 11
// global vars
int CASES_MONEY[MAX_CASES_SIZE]; // possible values inside a case, idx start at 1
int PLAYER_CASES[MAX_CASES_SIZE]; // index = case, value = money in case
int ROUNDS[MAX_ROUNDS_SIZE]; // index = round #, val = how many cases to choose in that round
int numCases = 0;
int totalCasesSelected = 0;
int numRounds = 0;
int selectedCase = 0;
int startingCase = 0;
int currRound = 1;

// function protos
string getDataString(int dataNum);
bool loadArray(int dataNum);
void initArray(int arr[], int size);
void printArray(int arr[], int size);
int generateRandom(int lower, int upper);
int printChooseNumCases();
bool isValidCases(int numCases);
bool isValidCase(int caseIdx);
bool isValidPlayerCase(int caseIdx);
void printMoney();
int printCases();
void chooseCase(int caseIdx);
void clearScreen();
float getAvgRemainingMoney();
float getOffer();

// main
int main() {
    int deal = 0;
    cout << "Welcome to Deal or No Deal!" << endl;
    numCases = 16;
    // numCases = printChooseNumCases();
    if (!loadArray(numCases)){
        cout << "error in loading arrays! exiting wrongfully." << endl;
        return -1;
    }

    printMoney();
    cout << "You must now pick a starting case." << endl;
    startingCase = printCases();
    chooseCase(startingCase);

    while (currRound <= numRounds && deal <= 0){
        clearScreen();
        int casesSelected = 0;
        cout << "We are in Round " << currRound << endl;
        cout << "You must choose " << ROUNDS[currRound] << " cases in this round." << endl;
        while(casesSelected <  ROUNDS[currRound] && deal <= 0){
            selectedCase = printCases();
            chooseCase(selectedCase);
            printMoney();
            casesSelected++;
            totalCasesSelected++;
        }
        if (currRound < numRounds){
            // do bank stuff
            float offer = getOffer();
            cout << "This is what the bank offers: " << offer << endl;
            cout << "Deal (1) or No Deal (0)? ";
            cin >> deal;
        }
        currRound++;
    }

    return 0;
}

// function decs
float getOffer(){
    float a = getAvgRemainingMoney();
    float b = (currRound * 1.0)/numRounds;
    return a * b;
}

float getAvgRemainingMoney(){
    float val = 0.0;
    for (int i = 1; i <= numCases; ++i) {
        if (CASES_MONEY[i] > 0){
            val += CASES_MONEY[i];
        }
    }
    return val/totalCasesSelected;
}

void chooseCase(int caseIdx) {
    int randIdx = generateRandom(1, numCases);
    while(CASES_MONEY[randIdx] < 0){
        randIdx = generateRandom(1, numCases);
    }
    PLAYER_CASES[caseIdx] = CASES_MONEY[randIdx];
    cout << "The value of case " << caseIdx << " is " << CASES_MONEY[randIdx] << endl;
    CASES_MONEY[randIdx] = -1;
}

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
    initArray(CASES_MONEY, MAX_CASES_SIZE);
    int line;
    df >> line;
    int i = 1;
    // load case money values
    while(i <= numCases){
        CASES_MONEY[i++] = line;
        df >> line;
    }

    // load rounds
    initArray(ROUNDS, MAX_ROUNDS_SIZE);
    numRounds = line;
    df >> line;
    i = 1;
    while(i <= numRounds){
        int val = line;
        ROUNDS[i++] = val;
        df >> line;
    }
    // init player cases
    initArray(PLAYER_CASES, MAX_CASES_SIZE);
    df.close();
    return true;
}

void initArray(int arr[], int size){
    for (int i = 0; i < size; ++i) {
        arr[i] = -1;
    }
}

int generateRandom(int lower, int upper){
    //srand(time(0));
    int random = lower + rand() % (upper - lower + 1);
    return random;
}

bool isValidCases(int cases) {
    bool val = true;
    if (cases % 2 != 0){
        val = false;
    }
    if (cases < MIN_CASES || cases > MAX_CASES){ // not in range
        val = false;
    }

    if (!val){
        cout << "Invalid number of cases chosen, try again (16, 18, 20, 22, 24 ,26): ";
    }
    return val;
}

bool isValidCase(int caseIdx) {
    if (caseIdx < 1 || caseIdx > numCases){ // not in range
        cout << "Invalid case chosen (out of range), try again: ";
        return false;
    }
    if (CASES_MONEY[caseIdx] < 0){ // invalid case chosen todo debug out of range index
        cout << "Invalid case chosen (case already chosen/does not exist), try again: ";
        return false;
    }
    return true;
}

bool isValidPlayerCase(int caseIdx) {
    if (caseIdx < 1 || caseIdx > numCases){ // not in range
        cout << "Invalid case chosen (out of range), try again: ";
        return false;
    }
    if (PLAYER_CASES[caseIdx] > 0){ // invalid case chosen todo debug out of range index
        cout << "Invalid case chosen (case already chosen/does not exist), try again: ";
        return false;
    }
    return true;
}


// gui functions
void clearScreen(){
    cout << string( 100, '\n' );
}

void printArray(int arr[], int size){
    for (int i = 0; i < size; ++i) {
        if (i % 5 == 0){
            cout << "\n";
        }
        cout << "\t" << arr[i];
    }
}

int printChooseNumCases(){
    int val;
    cout << "How many cases do you want in the game (16, 18, 20, 22, 24 ,26)? ";
    cin >> val;
    while (!isValidCases(val)){
        cin >> val;
    }
    cout << "\n";
    return val;
}

void printMoney(){
    int half = numCases/2;
    cout << "-------------------------" << endl;
    int i = 1;
    int k = half+1;
    while (i <= half){
        cout << "|\t" ;
        if (CASES_MONEY[i] > 0){
            cout << CASES_MONEY[i++];
        } else{
            cout << " ";
            i++;
        }
        cout << "\t";
        if (CASES_MONEY[k] > 0){
            cout << CASES_MONEY[k++];
        } else{
            cout << " ";
            k++;
        }
        cout << "\t|\n";
    }
    cout << "-------------------------\n" << endl;
}

int printCases(){
    cout << "=================================================" << endl;
    for (int i = 1; i <= numCases; ++i) {
        if ((i+4) % 5 == 0){ // start of new row?
            cout << "|\t";
        } else{
            cout << "\t";
        }
        if (PLAYER_CASES[i] < 0){ // case valid?
            cout << i;
        } else{ // case removed?
            cout << " ";
        }
        if (i % 5 == 0){ // end of row?
            cout << "\t|\n";
        }
        // print excess spaces if starting a new row
        if (i == numCases && i % 5 != 0){
            while (i % 5 != 0){
                cout << "\t" << " ";
                i++;
            }
            cout << "\t|" << endl;
        }
    }
    cout << "=================================================\n" << endl;
    int val;
    cout << "Please choose a case: ";
    cin >> val;
    while(!isValidPlayerCase(val)){
        cin >> val;
    }
    return val;
}
