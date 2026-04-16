#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class CampusCompass {
private:
    // Adjacency List Implementation since the graph is relatively sparse
    map<int, vector<pair<int,int>>> graph;
    // Also use other maps to keep track of location names and class locations
    map<int, string> locations;
    map<int, vector<vector<string>>> classes;
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    int ParseNumInputs(const string& input);
    bool ParseCommand(const string &input);
    bool insert(const string &student_name, int student_id, int residence_location_id, int N, vector<string> classcodes);
    bool remove(int student_id);
    bool dropClass(int student_id, string classcode);
    bool replaceClass(int student_id, string classcode_1, string classcode_2);
    bool removeClass(string classcode);
    bool toggleEdgeClosure(int N, vector<int> location_ids);
    bool checkEdgeStatus(int location_id_X, int location_id_Y);
    bool isConnected(int location_id_1, int location_id_2);
    void printShortestEdges(int id);
    void printStudentZone(int id);
    bool verifySchedule(int id);
};
