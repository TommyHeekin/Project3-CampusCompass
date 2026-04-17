#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    string input;

    int count = 0;
    while (count == 0) {
        getline(cin, input);
        count = compass.ParseNumInputs(input);
    }

    for (int i = 0; i < count; i++) {
        getline(cin, input);
        if (!compass.ParseCommand(input)) {
            cout << "unsuccessful" << endl;
        }
    }
    return 0;
}
