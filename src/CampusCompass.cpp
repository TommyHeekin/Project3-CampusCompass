#include "CampusCompass.h"

#include <string>
#include <vector>
#include <regex>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    return false;
}

bool CampusCompass::ParseCommand(const string &input, smatch& match, const regex& command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
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
