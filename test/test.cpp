#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <regex>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;

//Test 5 different incorrect commands
TEST_CASE("Incorrect Command Test", "[regex_test]") {

  CampusCompass compass;

  SECTION("Incorrect Name") {
    string input = "insert \"A11y\" 45679999 1 1 COP3530";
    REQUIRE(compass.ParseCommand(input) == false);
  }

  SECTION("Incorrect UFID") {
    string input = "insert \"Thomas Heekin\" 32 1 1 COP3530";
    REQUIRE(compass.ParseCommand(input) == false);
  }

  SECTION("Incorrect Location ID") {
    string input = "insert \"Thomas Heekin\" 45679999 A 1 COP3530";
    REQUIRE(compass.ParseCommand(input) == false);
  }

  SECTION("Incorrect Class Code") {
    string input = "removeClass 3530COP";
    REQUIRE(compass.ParseCommand(input) == false);
  }

  SECTION("Not enough parameters") {
    string input = "isConnected 23";
    REQUIRE(compass.ParseCommand(input) == false);
  }
}
// Test edge case in removeStudent function
TEST_CASE("Removing Student That Doesn't Exist", "[edge_case]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 33, 1, {"COP3530"});
  REQUIRE(compass.remove(45679999) == false);
}
// Test edge cases in dropClass function
TEST_CASE("Drop Class edge cases", "[edge_case]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 33, 1, {"COP3530"});
  REQUIRE(compass.dropClass(29824140, "CDA3101") == false);
  REQUIRE(compass.dropClass(29824140, "COP3530") == true);
  REQUIRE(compass.dropClass(29824140, "COP3530") == false);
}
// Test edge cases in replaceClass function
TEST_CASE("Replace Class edge cases", "[edge_case]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 33, 1, {"COP3530", "CDA3101"});
  REQUIRE(compass.replaceClass(45679999, "COP3530", "MAC2312") == false);
  REQUIRE(compass.replaceClass(29824140, "PHY2049", "MAC2312") == false);
  REQUIRE(compass.replaceClass(29824140, "COP3530", "CDA3101") == false);
  REQUIRE(compass.replaceClass(29824140, "COP3530", "MAC2312") == true);
}
// Test edge cases in removeClass function
TEST_CASE("Remove Class edge cases", "[edge_cases]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 33, 1, {"COP3530"});
  REQUIRE(compass.removeClass("CDA3101") == false);
  REQUIRE(compass.removeClass("COP3530") == true);
  REQUIRE(compass.remove(29824140) == false);
}

// Test dropClass, removeClass, remove, and replaceClass commands
TEST_CASE("Drop Class Function", "[command_test]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 33, 2, {"COP3530", "CDA3101"});
  REQUIRE(compass.replaceClass(29824140, "CDA3101", "MAC2312") == true);
  REQUIRE(compass.removeClass("MAC2312") == true);
  REQUIRE(compass.dropClass(29824140, "COP3530") == true);
  compass.insert("Thomas Heekin", 29824140, 33, 2, {"COP3530", "CDA3101"});
  REQUIRE(compass.remove(29824140) == true);
}

TEST_CASE("Print Edges Test", "[edge_case]") {
  CampusCompass compass;
  compass.insert("Thomas Heekin", 29824140, 4, 1, {"PHY2048"});
  //Still need to work on this one
}
