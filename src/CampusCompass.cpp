#include "CampusCompass.h"

#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

smatch match;
regex classcodePattern("(^[A-Z]{3}\\d{4}$)");
regex numberOfInputs = regex("^[0-9]+$");
regex insertCommand = regex("(^insert\\s)\"([A-Za-z ]+)\"\\s([0-9]{8})\\s([0-9]+)\\s([0-9]+)\\s(.*)");
regex removeCommand = regex("(^remove\\s)([0-9]{8})$");
regex dropClassCommand = regex("(^dropClass\\s)([0-9]{8})\\s([A-Z]{3})([0-9]{4})$");
regex replaceClassCommand = regex("(^replaceClass\\s)([0-9]{8})\\s([A-Z]{3})([0-9]{4})\\s([A-Z]{3})([0-9]{4})$");
regex removeClassCommand = regex("(^removeClass\\s)([A-Z]{3})([0-9]{4})$");
regex toggleClosureCommand = regex("(^toggleEdgesClosure\\s)([0-9]+)\\s(.*)");
regex checkStatusCommand = regex("(^checkEdgeStatus\\s)([0-9]+)\\s([0-9]+)$");
regex isConnectedCommand = regex("(^isConnected\\s)([0-9]+)\\s([0-9]+)$");
regex printShortestCommand = regex("(^printShortestEdges\\s)([0-9]{8})$");
regex printStudentCommand = regex("(^printStudentZone\\s)([0-9]{8})$");
regex verifyScheduleCommand = regex("(^verifySchedule\\s)([0-9]{8})$");

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    return false;
}

int CampusCompass::ParseNumInputs(const string &input) {
    if (regex_match(input, match, numberOfInputs)) {
        return stoi(match[0]);
    }
    return 0;
}


bool CampusCompass::ParseCommand(const string &input) {
    // Case 1: Insert Command
    if (regex_match(input, match, insertCommand)) {
        string student_name = match[2];
        int student_id = stoi(match[3]);
        int residence_id = stoi(match[4]);
        int N = stoi(match[5]);
        string classcodes_string = match[6];

        // Split the classcodes
        vector<string> classcodes;
        stringstream ss(classcodes_string);
        string code;
        while (ss >> code) {
            if (!regex_match(code, classcodePattern)) {
                return false;
            }
            classcodes.push_back(code);
        }

        if (classcodes.size() != N) {
            return false;
        }

        insert(student_name, student_id, residence_id, N, classcodes);
        return true;
    }

    // Case 2: Remove Command
    if (regex_match(input, match, removeCommand)) {
        int student_id = stoi(match[2]);
        remove(student_id);
        return true;
    }

    // Case 3: Drop Class Command
    if (regex_match(input, match, dropClassCommand)) {
        int student_id = stoi(match[2]);
        string classcode = match[3];
        dropClass(student_id, classcode);
        return true;
    }

    // Case 4: Replace Class Command
    if (regex_match(input, match, replaceClassCommand)) {
        int student_id = stoi(match[2]);
        string classcode1 = match[3];
        string classcode2 = match[4];
        replaceClass(student_id, classcode1, classcode2);
        return true;
    }

    // Case 5: Remove Class Command
    if (regex_match(input, match, removeClassCommand)) {
        string classcode = match[2];
        removeClass(classcode);
        return true;
    }

    // Case 6: Toggle Edge Closure Command
    if (regex_match(input, match, toggleClosureCommand)) {
        int N = stoi(match[2]);
        string locationIDString = match[3];
        vector<int> locationIDs;
        stringstream ss(locationIDString);
        string id;
        while (ss >> id) {
            locationIDs.push_back(stoi(id));
        }
        if (locationIDs.size() != N) {
            return false;
        }
        toggleEdgeClosure(N, locationIDs);
        return true;
    }

    // Case 7: Check Edge Status Command
    if (regex_match(input, match, checkStatusCommand)) {
        int locationIDX = stoi(match[2]);
        int locationIDY = stoi(match[3]);
        checkEdgeStatus(locationIDX, locationIDY);
        return true;
    }

    // Case 8: Is Connected Command
    if (regex_match(input, match, isConnectedCommand)) {
        int locationID1 = stoi(match[2]);
        int locationID2 = stoi(match[3]);
        isConnected(locationID1, locationID2);
        return true;
    }

    // Case 9: Print Shortest Edges Command
    if (regex_match(input, match, printShortestCommand)) {
        int id = stoi(match[2]);
        printShortestEdges(id);
        return true;
    }

    // Case 10: Print Student Zone Command
    if (regex_match(input, match, printStudentCommand)) {
        int id = stoi(match[2]);
        printStudentZone(id);
        return true;
    }

    // Case 11: Verify Schedule Command
    if (regex_match(input, match, verifyScheduleCommand)) {
        int id = stoi(match[2]);
        verifySchedule(id);
        return true;
    }

    return false;
}

bool CampusCompass::insert(const string &student_name, int student_id, int residence_location_id, int N, vector<string> classcodes) {
    return false;
}

bool CampusCompass::remove(int student_id) {
    return false;
}

bool CampusCompass::dropClass(int student_id, string classcode) {
    return false;
}

bool CampusCompass::replaceClass(int student_id, string classcode1, string classcode2) {
    return false;
}

bool CampusCompass::removeClass(string classcode) {
    return false;
}

bool CampusCompass::toggleEdgeClosure(int N, vector<int> location_ids) {
    return false;
}

bool CampusCompass::checkEdgeStatus(int location_id_X, int location_id_Y) {
    return false;
}

bool CampusCompass::isConnected(int location_id_1, int location_id_2) {
    return false;
}

void CampusCompass::printShortestEdges(int id) {

}

void CampusCompass::printStudentZone(int id) {

}

bool CampusCompass::verifySchedule(int id) {
    return false;
}
