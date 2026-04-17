#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>

using namespace std;

smatch match;
regex classcodePattern("(^[A-Z]{3}\\d{4}$)");
regex numberOfInputs = regex("^[0-9]+$");
regex insertCommand = regex("(^insert\\s)\"([A-Za-z ]+)\"\\s([0-9]{8})\\s([0-9]+)\\s([0-9]+)\\s(.*)");
regex removeCommand = regex("(^remove\\s)([0-9]{8})$");
regex dropClassCommand = regex("(^dropClass\\s)([0-9]{8})\\s([A-Z]{3}[0-9]{4})$");
regex replaceClassCommand = regex("(^replaceClass\\s)([0-9]{8})\\s([A-Z]{3}[0-9]{4})\\s([A-Z]{3}[0-9]{4})$");
regex removeClassCommand = regex("(^removeClass\\s)([A-Z]{3}[0-9]{4})$");
regex toggleClosureCommand = regex("(^toggleEdgesClosure\\s)([0-9]+)\\s(.*)");
regex checkStatusCommand = regex("(^checkEdgeStatus\\s)([0-9]+)\\s([0-9]+)$");
regex isConnectedCommand = regex("(^isConnected\\s)([0-9]+)\\s([0-9]+)$");
regex printShortestCommand = regex("(^printShortestEdges\\s)([0-9]{8})$");
regex printStudentCommand = regex("(^printStudentZone\\s)([0-9]{8})$");
regex verifyScheduleCommand = regex("(^verifySchedule\\s)([0-9]{8})$");

// Constructor
CampusCompass::CampusCompass() = default;

// Parse data from CSV files
bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // First parse edges.csv
    ifstream file(edges_filepath);
    if (!file.is_open()) return false;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        // Read data from a given line of the csv file
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> data;
        string str;
        while (getline(ss, str, ',')) {
            data.push_back(str);
        }
        if (data.size() != 5) continue;

        // Parse the ids, names, and weight from the line
        int locationID1 = stoi(data[0]);
        int locationID2 = stoi(data[1]);
        string name1 = data[2];
        string name2 = data[3];
        int time = stoi(data[4]);

        // Add information to maps
        if (locations.find(locationID1) == locations.end()) {
            locations[locationID1] = name1;
        }
        if (locations.find(locationID2) == locations.end()) {
            locations[locationID2] = name2;
        }

        graph[locationID1].push_back({locationID2, time});
        graph[locationID2].push_back({locationID1, time});
    }

    // Then parse classes.csv
    ifstream file2(classes_filepath);
    if (!file2.is_open()) return false;
    getline(file2, line);
    while (getline(file2, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> data;
        string str;
        while (getline(ss, str, ',')) {
            data.push_back(str);
        }
        if (data.size() != 4) continue;
        string classcode = data[0];
        int locationID = stoi(data[1]);
        string startTime = data[2];
        string endTime = data[3];

        classes[locationID].push_back({classcode, startTime, endTime});
        valid_classcodes.insert(classcode);
    }

    return true;
}

// Parses the number of commands that will follow in the terminal
int CampusCompass::ParseNumInputs(const string &input) {
    if (regex_match(input, match, numberOfInputs)) {
        return stoi(match[0]);
    }
    return 0;
}

// Parses the commands from user input
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

        if (insert(student_name, student_id, residence_id, classcodes)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
        return true;
    }

    // Case 2: Remove Command
    if (regex_match(input, match, removeCommand)) {
        int student_id = stoi(match[2]);
        if (remove(student_id)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
        return true;
    }

    // To Do: Validate class codes
    // Case 3: Drop Class Command
    if (regex_match(input, match, dropClassCommand)) {
        int student_id = stoi(match[2]);
        string classcode = match[3];
        if (dropClass(student_id, classcode)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
        return true;
    }

    // Case 4: Replace Class Command
    if (regex_match(input, match, replaceClassCommand)) {
        int student_id = stoi(match[2]);
        string classcode1 = match[3];
        string classcode2 = match[4];
        if (!replaceClass(student_id, classcode1, classcode2)) {
            cout << "unsuccessful" << endl;
        }
        return true;
    }

    // Case 5: Remove Class Command
    if (regex_match(input, match, removeClassCommand)) {
        string classcode = match[2];
        if (removeClass(classcode)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
        return true;
    }

    // Case 6: Toggle Edge Closure Command
    if (regex_match(input, match, toggleClosureCommand)) {
        int N = stoi(match[2]);
        string locationIDString = match[3];
        vector<int> locationIDs;
        stringstream ss(locationIDString);
        string id;
        // Split locations IDs
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

// Insert a student and add them to the specified classes
bool CampusCompass::insert(const string &student_name, int student_id, int residence_location_id, vector<string> classcodes) {
    // Insertion fails if student id already exists
    if (students.find(student_id) != students.end()) {
        return false;
    }

    // Check if all classcodes are valid
    for (const string& classcode : classcodes) {
        if (valid_classcodes.find(classcode) == valid_classcodes.end()) {
            return false;
        }
    }

    // Otherwise add student
    students[student_id] = {student_name, residence_location_id, classcodes};

    return true;
}

// Remove a student from all classes
bool CampusCompass::remove(int student_id) {
    // If student id doesn't exist, remove fails
    if (students.find(student_id) == students.end()) {
        return false;
    }
    // Remove student from map
    students.erase(student_id);

    return true;
}

// Remove a given classcode for a student
bool CampusCompass::dropClass(int student_id, const string& classcode) {
    // Fails if the student id does not exist
    if (students.find(student_id) == students.end()) {
        return false;
    }

    // Also if student does not have the given classcode, drop fails
    bool class_found = false;
    vector<string>& studentClasses = get<2>(students[student_id]);
    for (string studentClass : studentClasses) {
        if (studentClass == classcode) {
            class_found = true;
        }
    }
    if (!class_found) {
        return false;
    }

    // Remove classcode from student
    studentClasses.erase(std::remove(studentClasses.begin(), studentClasses.end(), classcode), studentClasses.end());
    if (studentClasses.empty()) {
        students.erase(student_id);
    }
    return true;
}

// Replaces a class with another class in the student's schedule
bool CampusCompass::replaceClass(int student_id, const string& classcode1, const string& classcode2) {
    // Check if classcode2 is valid
    if (valid_classcodes.find(classcode2) == valid_classcodes.end()) {
        return false;
    }

    // Make sure replace doesnt cause duplicate classes
    auto& studentClasses = get<2>(students[student_id]);
    if (find(studentClasses.begin(), studentClasses.end(), classcode2) != studentClasses.end()) {
        return false;
    }

    // Call dropclass function to drop classcode 1
    // If dropclass fails, return false
    if (!dropClass(student_id, classcode1)) {
        return false;
    }

    // If so, add student to class
    get<2>(students[student_id]).push_back(classcode2);

    return true;
}

// Removes a classcode from the schedule for all students
bool CampusCompass::removeClass(const string& classcode) {
    // Remove classcode from every student
    int count = 0;
    for (auto it = students.begin(); it != students.end();) {
        auto& studentClasses = get<2>(it->second);

        auto it2 = find(studentClasses.begin(), studentClasses.end(), classcode);
        if (it2 != studentClasses.end()) {
            studentClasses.erase(it2);
            count++;
        }

        // If a student has no classes, delete them
        if (studentClasses.empty()) {
            it = students.erase(it);
        } else {
            ++it;
        }
    }

    // If a class isn't dropped from any students, remove fails
    if (count == 0) {
        return false;
    }

    cout << count << endl;
    return true;
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
