#include "CampusCompass.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <queue>
#include <unordered_set>
#include <climits>
#include <unordered_map>

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
regex printShortestCommand = regex("(^printShortestEdges\\s)([0-9]{8})\\s*$");
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
    bool edgeOpen = true;
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

        graph[locationID1].push_back({locationID2, time, edgeOpen});
        graph[locationID2].push_back({locationID1, time, edgeOpen});
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

        classes[classcode] = {locationID, startTime, endTime};
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

        if (classcodes.size() != N || classcodes.size() > 6 || classcodes.size() < 1) {
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
        if (replaceClass(student_id, classcode1, classcode2)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
        return true;
    }

    // Case 5: Remove Class Command
    if (regex_match(input, match, removeClassCommand)) {
        string classcode = match[2];
        if (!removeClass(classcode)) {
            cout << "unsuccessful" << endl;
        }
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
        if (locationIDs.size() != (N * 2)) {
            return false;
        }
        toggleEdgeClosure(locationIDs);
        cout << "successful" << endl;
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
        if (isConnected(locationID1, locationID2)) {
            cout << "successful" << endl;
        } else cout << "unsuccessful" << endl;
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
    auto& studentClasses = get<2>(students[student_id]);

    // Check if classcode2 is valid
    if (valid_classcodes.find(classcode2) == valid_classcodes.end()) {
        return false;
    }

    // Make sure the student doesn't already have classcode 2
    if (find(studentClasses.begin(), studentClasses.end(), classcode2) != studentClasses.end()) {
        return false;
    }

    // Call dropClass function to drop classcode1
    // If this function returns false, replace fails
    if (!dropClass(student_id, classcode1)) {
        return false;
    }

    // Now update new classes
    auto& newClasses = get<2>(students[student_id]);
    newClasses.push_back(classcode2);

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

// Toggle the given edges open or closed
bool CampusCompass::toggleEdgeClosure(vector<int> location_ids) {
    for (int i = 0; i < location_ids.size(); i += 2) {
        int id1 = location_ids[i];
        int id2 = location_ids[i + 1];

        // Toggle edgeOpen variable for edge between two ids
        for (auto& edge : graph[id1]) {
            if (get<0>(edge) == id2) {
                get<2>(edge) = !get<2>(edge);
                break;
            }
        }

        // Also toggle edge in other direction
        for (auto& edge : graph[id2]) {
            if (get<0>(edge) == id1) {
                get<2>(edge) = !get<2>(edge);
                break;
            }
        }
    }
    return true;
}

// Check if a current edge is open, closed, or does not exist
void CampusCompass::checkEdgeStatus(int location_id_X, int location_id_Y) {
    // Check if edge is connected and output to terminal accordingly
    for (auto& edge : graph[location_id_X]) {
        if (get<0>(edge) == location_id_Y) {
            if (get<2>(edge)) cout << "open" << endl;
            else cout << "closed" << endl;
            return;
        }
    }
    cout << "DNE" << endl;
}

// Check if it's possible to travel between two locations
bool CampusCompass::isConnected(int location_id_1, int location_id_2) {
    // Finding if the two locations are connected using BFS
    unordered_set<int> visited;
    queue<int> vertex_queue;
    vertex_queue.push(location_id_1);

    //Main loop
    while (!vertex_queue.empty()) {
        int curr = vertex_queue.front();
        vertex_queue.pop();
        // Return true if location 2 is reached
        if (curr == location_id_2) {
            return true;
        }

        // Otherwise, add all neighbors to the queue
        for (const auto& edge : graph[curr]) {
            int neighbor = get<0>(edge);
            bool edgeOpen = get<2>(edge);

            // Add to queue only if edge is currently open
            if (edgeOpen && visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                vertex_queue.push(neighbor);
            }
        }
    }
    return false;
}

// Prints the shortest walking time from a student residence to every class
void CampusCompass::printShortestEdges(int id) {
    string studentName = get<0>(students[id]);
    int residenceID = get<1>(students[id]);
    vector<string> classcodes = get<2>(students[id]);
    sort(classcodes.begin(), classcodes.end());

    // Call helper function
    vector<unordered_map<int,int>> result = djikstrasAlgorithm(residenceID);

    // Print results to terminal
    // Classes are stored in a sorted set
    cout << "Time For Shortest Edges: " << studentName << endl;
    for (const string& classcode : classcodes) {
        int location = get<0>(classes[classcode]);
        if (result[0][location] == INT_MAX) {
            cout << classcode << ": -1" << endl;
        } else cout << classcode << ": " << result[0][location] << endl;
    }
}

// Creates a MST and outputs the total coast
void CampusCompass::printStudentZone(int id) {
    string studentName = get<0>(students[id]);
    int residenceID = get<1>(students[id]);
    vector<string> classcodes = get<2>(students[id]);

    // Call Djikstra helper function
    vector<unordered_map<int,int>> result = djikstrasAlgorithm(residenceID);

    // Get all vertices in the shortest paths
    unordered_set<int> vertices;
    vertices.insert(residenceID);

    for (const string& classcode : classcodes) {
        int location = get<0>(classes[classcode]);
        int currID = location;

        while (currID != residenceID) {
            vertices.insert(currID);
            currID = result[1][currID];
        }
        vertices.insert(residenceID);
    }

    // Use Prim's algorithm to create the MST
    int totalCost = 0;
    unordered_set<int> visited;

    // Initialize the min heap
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> primQueue;
    primQueue.push({0, residenceID});

    while (!primQueue.empty()) {
        int currDistance = primQueue.top().first;
        int currID = primQueue.top().second;
        primQueue.pop();

        if (visited.find(currID) != visited.end()) continue;

        visited.insert(currID);
        totalCost += currDistance;

        for (const auto& edge : graph[currID]) {
            int neighbor = get<0>(edge);
            int distance = get<1>(edge);
            bool edgeOpen = get<2>(edge);

            if (edgeOpen && vertices.count(neighbor) && !visited.count(neighbor)) {
                primQueue.push({distance, neighbor});
            }
        }
    }

    // Output total cost to terminal
    cout << "Student Zone Cost For " << studentName << ": " << totalCost << endl;
}

bool CampusCompass::verifySchedule(int id) {
    string studentName = get<0>(students[id]);
    vector<string> classcodes = get<2>(students[id]);

    // Function fails if student only has 1 class
    if (classcodes.size() <= 1){
        cout << "unsuccessful" << endl;
        return false;
    }

    // Sort classes by start time
    vector<pair<string,string>> temp;
    for (const string& classcode : classcodes) {
        string startTime = get<1>(classes[classcode]);
        temp.push_back({startTime, classcode});
    }
    sort(temp.begin(), temp.end());
    classcodes.clear();
    for (auto& pair : temp) {
        classcodes.push_back(pair.second);
    }

    // Verify consecutive classes, if the time gap isn't large enough return unsuccessful
    for (int i = 0; i < classcodes.size()-1; i++) {
        // Get current and next class
        string class1 = classcodes[i];
        string class2 = classcodes[i+1];
        int id1 = get<0>(classes[class1]);
        int id2 = get<0>(classes[class2]);
        // Also get endTime of class1 and startTime of class using helper function
        int endTime1 = stoi(get<2>(classes[class1]).substr(0,2)) * 60 + stoi(get<2>(classes[class1]).substr(3,2));
        int startTime2 = stoi(get<1>(classes[class2]).substr(0,2)) * 60 + stoi(get<1>(classes[class2]).substr(3,2));
        // Calculate gap between two times in minutes
        int timeGap = startTime2 - endTime1;

        // Use Djikstra's helper function for the first id
        vector<unordered_map<int,int>> result = djikstrasAlgorithm(id1);
        int travelTime = result[0][id2];

        // If the result of djikstra's is less than the gap between classes, schedule is valid
        if (travelTime != INT_MAX && travelTime <= timeGap) {
            cout << class1 << " - " << class2 << ": successful" << endl;
        } else cout << class1 << " - " << class2 << ": unsuccessful" << endl;
    }
    return true;
}

vector<unordered_map<int,int>> CampusCompass::djikstrasAlgorithm(int residenceID) {
    unordered_map<int,int> distance;
    unordered_map<int,int> predecessor;

    // Start by setting distance to infinity for all nodes
    for (const auto& [key,_] : graph) {
        distance[key] = INT_MAX;
    }
    // Set distance for starting node to zero
    distance[residenceID] = 0;

    // Initialize priority queue for main loop
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> djQueue;
    djQueue.push({0, residenceID});

    // Main loop
    while (!djQueue.empty()) {
        int currDistance = djQueue.top().first;
        int currID = djQueue.top().second;
        djQueue.pop();

        // If distance to current node is greater, skip
        if (currDistance > distance[currID]) continue;

        // Otherwise update
        for (const auto& edge : graph[currID]) {
            int neighbor = get<0>(edge);
            int weight = get<1>(edge);
            bool edgeOpen = get<2>(edge);

            // Skip if edge is not open
            if (!edgeOpen) continue;

            // Otherwise update with new distance
            int newDistance = currDistance + weight;
            if (newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                predecessor[neighbor] = currID;
                djQueue.push({newDistance, neighbor});
            }
        }
    }

    return {distance, predecessor};
}
