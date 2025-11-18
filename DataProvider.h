#pragma once
#ifdef DATAPROVIDER_EXPORTS
#define LIBSPEC extern "C" __declspec(dllexport)
#else
#define LIBSPEC extern "C" __declspec(dllimport)
#endif
LIBSPEC void* GetItem(int iItem, char* pID = nullptr);

// Function GetItem() generates a stand-alone item of type iItem and returns the pointer
// to it. 
// If the second parameter of this function is zero pointer,
// the pID is selected from list Colors.txt. If not, the string presented by the second
// parameter is used.
// Examples:
// Creates data structure of type 3 consisting of 100 items of type 10
// ITEM5 *pit5 = (ITEM5 *)GetItem(5, (char *)"Merekarva Roheline");
// Constructs item of type 5, ID will set to "Merekarva Roheline" (do not forget that "Merekarva Roheline" is of type const string *)
// ITEM8 *pit8 = (ITEM8 *)GetItem(8);
// Constructs item of type 8, ID will set by program
// In case of failure GetItem() throws exception 

