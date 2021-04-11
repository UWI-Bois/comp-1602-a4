#include <iostream>
#include <fstream>
using namespace std;

const string DATA_DIR = "datafiles/";

bool dataFileExists(const string& fileName);

int main() {
    cout << "hi good day" << endl;
    cout << DATA_DIR << endl;
    cout << dataFileExists("Data16.txt") << endl;

    return 0;
}

bool dataFileExists(const string& fileName){
    ifstream df;
    df.open(DATA_DIR + fileName);
    if (df.is_open()){
        return true;
    } else{
        return false;
    }
}
