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
    map<int, vector<tuple<int,int,bool>>> graph;
    // Also use other maps to keep track of students and locations
    map<int, tuple<string, int, vector<string>>> students;
    map<int, string> locations;

    // Set to keep track of valid classcodes
    set<string> valid_classcodes;

    // For extra credit
    map<string, tuple<int, string, string>> classes;
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    int ParseNumInputs(const string& input);
    bool ParseCommand(const string &input);
    bool insert(const string &student_name, int student_id, int residence_location_id, vector<string> classcodes);
    bool remove(int student_id);
    bool dropClass(int student_id, const string& classcode);
    bool replaceClass(int student_id, const string& classcode_1, const string& classcode_2);
    bool removeClass(const string& classcode);
    bool toggleEdgeClosure(vector<int> location_ids);
    void checkEdgeStatus(int location_id_X, int location_id_Y);
    bool isConnected(int location_id_1, int location_id_2);
    void printShortestEdges(int id);
    void printStudentZone(int id);
    bool verifySchedule(int id);

    // Also have helper functions for Djikstra's and Prim's Algorithms
    vector<unordered_map<int,int>> djikstrasAlgorithm(int residenceID);
};
