#include <iostream>
#include <fstream>
#include <string>
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
int MONEY_CASES[MAX_CASES_SIZE]; // possible values inside a case, idx start at 1
int PLAYER_CASES[MAX_CASES_SIZE]; // index = case, value = money in case
int ROUNDS[MAX_ROUNDS_SIZE]; // index = round #, val = how many cases to choose in that round
int numCases = 0;
int totalCasesSelected = 0;
int numRounds = 0;
int startingCaseIdx = 0;
int startingMoneyIdx = 0;
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
void chooseFirstCase(int caseIdx);
void swapCases(int selectedCase);

// main
int main() {
    int deal = 0;
    float offer = 0.0;
    cout << "Welcome to Deal or No Deal!" << endl;
    numCases = 26;
    // numCases = printChooseNumCases();
    if (!loadArray(numCases)){
        cout << "error in loading arrays! exiting wrongfully." << endl;
        return -1;
    }

    printMoney();
    cout << "You must now pick a starting case." << endl;
    startingCaseIdx = printCases();
    chooseFirstCase(startingCaseIdx);

    while (currRound <= numRounds && deal <= 0){
        clearScreen();
        int casesSelected = 0;
        int selectedCase = 0;
        cout << "We are in Round " << currRound << endl;
        cout << "You must choose " << ROUNDS[currRound] << " cases in this round." << endl;
        while(casesSelected <  ROUNDS[currRound]){
            selectedCase = printCases();
            chooseCase(selectedCase);
            printMoney();
            casesSelected++;
        }
        currRound++;
        if (currRound < numRounds){ // accept the banker's offer?
            // do bank stuff
            offer = getOffer();
            cout << "This is what the bank offers: " << offer << endl;
            cout << "Deal (1) or No Deal (0)? ";
            cin >> deal;
            if (deal == 1){
                cout << "\nCongratulations! You have won $" << offer << endl;
                cout << "\nThanks for playing! Come back soon!" << endl;
                return 0;
            }
        } else{ // we are in the last round and down to the last case
            cout << "Do you want to keep your case (1)\nOr do you want to swap it with the last case (0)? ";
            int swap = 0;
            cin >> swap;
            if (swap == 1){
                swapCases(selectedCase);
            }
            cout << "The value of case " << startingCaseIdx << " is " << PLAYER_CASES[startingCaseIdx] << endl;
            cout << "\nCongratulations! You have won $" << PLAYER_CASES[startingCaseIdx] << endl;
            cout << "\nThanks for playing! Come back soon!" << endl;
            return 0;
        }
    }

    return 0;
}

void swapCases(int selectedCase) {
    // we assume at this point we are at the end
    cout << "Swapping case " << startingCaseIdx << " with " << selectedCase << endl;
    int lastMoneyIdx = 0;
    while(MONEY_CASES[++lastMoneyIdx] <= 0 && lastMoneyIdx != startingMoneyIdx);

    PLAYER_CASES[selectedCase] = MONEY_CASES[lastMoneyIdx];
    MONEY_CASES[lastMoneyIdx] = -1;

    MONEY_CASES[startingMoneyIdx] = PLAYER_CASES[startingCaseIdx];
    PLAYER_CASES[startingCaseIdx] = -1;
    cout << "Your case is now" << selectedCase << endl;
    startingCaseIdx = selectedCase;
}

// function decs
float getOffer(){
    float a = getAvgRemainingMoney();
    float b = (currRound * 1.0)/(numRounds * 1.0);
    return a * b;
}

float getAvgRemainingMoney(){
    float val = 0.0;
    int count = 0;
    for (int i = 1; i <= numCases; ++i) {
        if (MONEY_CASES[i] > 0){
            val += MONEY_CASES[i] * 1.0;
            count++;
        }
    }
    return val/(count) * 1.0;
}

void chooseCase(int caseIdx) {
    int randMoneyIdx = generateRandom(1, numCases);
    while(MONEY_CASES[randMoneyIdx] < 0 && randMoneyIdx != startingMoneyIdx){
        randMoneyIdx = generateRandom(1, numCases);
    }
    PLAYER_CASES[caseIdx] = MONEY_CASES[randMoneyIdx];
    cout << "The value of case " << caseIdx << " is " << MONEY_CASES[randMoneyIdx] << endl;
    MONEY_CASES[randMoneyIdx] = -1;
    totalCasesSelected++;
}

void chooseFirstCase(int caseIdx) {
    int randMoneyIdx = generateRandom(1, numCases);
    while(MONEY_CASES[randMoneyIdx] < 0){
        randMoneyIdx = generateRandom(1, numCases);
    }
    PLAYER_CASES[caseIdx] = MONEY_CASES[randMoneyIdx];
    startingMoneyIdx = randMoneyIdx;
    startingCaseIdx = caseIdx;
    totalCasesSelected++;
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
    initArray(MONEY_CASES, MAX_CASES_SIZE);
    int line;
    df >> line;
    int i = 1;
    // load case money values
    while(i <= numCases){
        MONEY_CASES[i++] = line;
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
    if (MONEY_CASES[caseIdx] < 0){ // invalid case chosen todo debug out of range index
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
        if (MONEY_CASES[i] > 0){
            cout << MONEY_CASES[i++];
        } else{
            cout << " ";
            i++;
        }
        cout << "\t";
        if (MONEY_CASES[k] > 0){
            cout << MONEY_CASES[k++];
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
