// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include <regex>
//
// // change if you choose to use a different header name
// #include "CampusCompass.h"
//
// using namespace std;
//
// //Test 5 different incorrect commands
// TEST_CASE("Incorrect Command Test", "[regex_test]") {
//
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//
//   SECTION("Incorrect Name") {
//     string input = "insert \"A11y\" 45679999 1 1 COP3530";
//     REQUIRE(compass.ParseCommand(input) == false);
//   }
//
//   SECTION("Incorrect UFID") {
//     string input = "insert \"Thomas Heekin\" 32 1 1 COP3530";
//     REQUIRE(compass.ParseCommand(input) == false);
//   }
//
//   SECTION("Incorrect Location ID") {
//     string input = "insert \"Thomas Heekin\" 45679999 A 1 COP3530";
//     REQUIRE(compass.ParseCommand(input) == false);
//   }
//
//   SECTION("Incorrect Class Code") {
//     string input = "removeClass 3530COP";
//     REQUIRE(compass.ParseCommand(input) == false);
//   }
//
//   SECTION("Not enough parameters") {
//     string input = "isConnected 23";
//     REQUIRE(compass.ParseCommand(input) == false);
//   }
// }
//
// // Test edge cases
// TEST_CASE("Test at least three edge cases for various functions", "[edge_case]") {
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//
//   SECTION("Removing Student That Doesn't Exist") {
//     compass.insert("Thomas Heekin", 29824140, 33, {"COP3530"});
//     REQUIRE(compass.remove(45679999) == false);
//   }
//
//   SECTION("Drop Class edge cases") {
//     compass.insert("Thomas Heekin", 29824140, 33, {"COP3530"});
//     REQUIRE(compass.dropClass(29824140, "CDA3101") == false);
//     REQUIRE(compass.dropClass(29824140, "COP3530") == true);
//     REQUIRE(compass.dropClass(29824140, "COP3530") == false);
//   }
//
//   SECTION("Replace Class edge cases") {
//     compass.insert("Thomas Heekin", 29824140, 33, {"COP3530", "CDA3101"});
//     REQUIRE(compass.replaceClass(45679999, "COP3530", "MAC2312") == false);
//     REQUIRE(compass.replaceClass(29824140, "PHY2049", "MAC2312") == false);
//     REQUIRE(compass.replaceClass(29824140, "COP3530", "CDA3101") == false);
//     REQUIRE(compass.replaceClass(29824140, "COP3530", "MAC2312") == true);
//   }
//
//   SECTION("Remove Class edge cases") {
//     compass.insert("Thomas Heekin", 29824140, 33, {"COP3530"});
//     REQUIRE(compass.removeClass("CDA3101") == false);
//     REQUIRE(compass.removeClass("COP3530") == true);
//     REQUIRE(compass.remove(29824140) == false);
//   }
// }
//
// // Test dropClass, removeClass, remove, and replaceClass commands
// TEST_CASE("Drop Class Function", "[command_test]") {
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//   compass.insert("Thomas Heekin", 29824140, 33, {"COP3530", "CDA3101"});
//   REQUIRE(compass.replaceClass(29824140, "CDA3101", "MAC2312") == true);
//   REQUIRE(compass.removeClass("MAC2312") == true);
//   REQUIRE(compass.dropClass(29824140, "COP3530") == true);
//   compass.insert("Thomas Heekin", 29824140, 33, {"COP3530", "CDA3101"});
//   REQUIRE(compass.remove(29824140) == true);
// }
//
// // Test your printShortestEdges command in a case where a student can reach a class, then edges turn off and they cannot
// TEST_CASE("Print Edges Test", "[edge_case]") {
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//   compass.insert("Thomas Heekin", 29824140, 4, {"PHY2048", "COP3530"});
//   REQUIRE(compass.printShortestEdges(29824140) == vector<pair<string,int>>{{"COP3530", 21}, {"PHY2048", 18}});
//   compass.toggleEdgeClosure({49, 56});
//   REQUIRE(compass.printShortestEdges(29824140) == vector<pair<string,int>>{{"COP3530", 21}, {"PHY2048", -1}});
// }
//
// // Additional edge case test
// TEST_CASE("Insert Duplicate IDs", "[edge_case], [command_test]") {
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//   REQUIRE(compass.insert("Thomas Heekin", 29824140, 33, {"COP3530"}) == true);
//   REQUIRE(compass.insert("Billy Bob", 29824140, 2, {"CDA3101"}) == false);
// }
