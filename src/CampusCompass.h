#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

using namespace std;

class CampusCompass {
private:
    // Adjacency List Implementation since the graph is relatively sparse
    // Key: From ID, Value : {To ID, Distance, EdgeOpen}
    map<int, vector<tuple<int,int,bool>>> graph;

    // Also use other maps to keep track of students and locations
    // Key: Student ID, Value: {Student Name, Residence ID, Vector of Class codes}
    map<int, tuple<string, int, vector<string>>> students;
    // Key: Location ID, Value: Location Name
    map<int, string> locations;

    // Set to keep track of valid classcodes
    set<string> valid_classcodes;

    // For extra credit : Maps classcodes to their location ids, start times, and end times
    map<string, tuple<int, string, string>> classes;
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    static int ParseNumInputs(const string& input);
    bool ParseCommand(const string &input);
    bool insert(const string &student_name, int student_id, int residence_location_id, vector<string> classcodes);
    bool remove(int student_id);
    bool dropClass(int student_id, const string& classcode);
    bool replaceClass(int student_id, const string& classcode_1, const string& classcode_2);
    bool removeClass(const string& classcode);
    bool toggleEdgeClosure(const vector<int>& location_ids);
    void checkEdgeStatus(int location_id_X, int location_id_Y);
    bool isConnected(int location_id_1, int location_id_2);
    vector<pair<string,int>> printShortestEdges(int id);
    void printStudentZone(int id);
    bool verifySchedule(int id);

    // Also have helper function for Djikstra's Algorithm
    vector<unordered_map<int,int>> djikstrasAlgorithm(int residenceID);
};
