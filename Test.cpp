#include "Coursework2.h"

#include <cstring>
#include <iostream>

void EvaluationTest() {
  using namespace std;

  char c;
  const char* colours[] = {
      "Antique Fuchsia", "Oxford Blue",  "Outrageous Orange",
      "Cameo Pink",      "Cyan Process", "Charm Pink",
      "Caladon Blue",    "Cadet Blue",   "Carnation Pink"};
  char buf[20];
  bool test1Passed = false;
  bool test2Passed = false;
  bool test3Passed = false;
  //
  // Test 1
  //
  cout << "Test 1 (Item class): running..." << endl;
  try {
    Item it1(nullptr);   // testing constructor
    cout << it1 << endl; // testing printout
    Item* pit = new Item(nullptr);
    cout << *pit << endl;
    delete pit;     // testing destructor
    Item it2 = it1; // testing copy constructor
    cout << it2 << endl;
    Item it3;  // testing default constructor
    it3 = it1; // testing assignment
    cout << it3 << endl;
    cout << boolalpha << (it3 == it2) << endl; // testing operator==
    test1Passed = true;
  } catch (exception& e) {
    cout << "Exception in Test 1: " << e.what() << endl;
  }
  cout << "Test 1 (Item class): " << (test1Passed ? "passed" : "failed")
       << endl;
  //
  // Test 2
  //
  cout << endl << "Test 2 (DataStructure basics): running..." << endl;
  try {
    DataStructure* pds1 = new DataStructure(); // test constructor
    int added = 0;
    for (int i = 0; i < 35; i++) {
      try {
        Item it(nullptr);
        *pds1 += it; // test operator+=
        added++;
      } catch (exception& e) {
        cout << e.what() << endl;
      }
    }
    cout << endl << *pds1 << endl; // test printout
    int num = pds1->GetItemsNumber();
    cout << "Number of items is " << num << endl; // test number of items
    test2Passed = (num == added);
    delete pds1; // test destructor
  } catch (exception& e) {
    cout << "Exception in Test 2: " << e.what() << endl;
  }
  cout << "Test 2 (DataStructure basics): "
       << (test2Passed ? "passed" : "failed") << endl;
  //
  // Test 3
  //
  cout << endl << "Test 3 (DataStructure remove): running..." << endl;
  try {
    DataStructure* pds2 = new DataStructure();
    for (int i = 0; i < 9; i++) {
      try {
        strcpy_s(buf, colours[i]);
        Item it(buf);
        ;
        *pds2 += it; // test operator+=
      } catch (exception& e) {
        cout << e.what() << endl;
      }
    }
    cout << endl << *pds2 << endl;
    for (int i = 0; i < 9; i++) {
      try {
        strcpy_s(buf, colours[i]);
        *pds2 -= buf;
        cout << endl << *pds2 << endl;
      } catch (exception& e) {
        cout << e.what() << endl;
      }
    }
    int finalNum = pds2->GetItemsNumber();
    cout << finalNum << endl;
    test3Passed = (finalNum == 0);
    delete pds2;
  } catch (exception& e) {
    cout << "Exception in Test 3: " << e.what() << endl;
  }
  cout << "Test 3 (DataStructure remove): "
       << (test3Passed ? "passed" : "failed") << endl;
}