#pragma once

#include "Item.h"

#include <array>
#include <forward_list>
#include <map>
#include <ostream>

// =============================================================================
// DataStructure: A two-level bucketed container for storing Item objects.
//
// This data structure organizes items using a two-level indexing scheme based
// on the item's ID string, which has the format "FirstWord SecondWord"
// (e.g., "Cafe Noir", "Tiffany Blue").
//
// Storage organization:
// - Level 1: A map keyed by the first letter of the first word (A-Z)
// - Level 2: An array of 26 linked lists, indexed by the first letter
//            of the second word (A=0, B=1, ... Z=25)
//
// Example: "Cafe Noir" would be stored at mBuckets['C'][13]
//          where 'C' is from "Cafe" and 13 is the index for 'N' in "Noir"
// =============================================================================
class DataStructure
{
private:
  // A Bucket is an array of 26 linked lists (one for each letter A-Z).
  // The index corresponds to the first letter of the second word in an item's ID.
  using Bucket = std::array<std::forward_list<Item>, 26>;

  // Map from first letter of the first word -> Bucket containing items.
  // Example: mBuckets['C'] contains all items whose ID starts with 'C'.
  std::map<char, Bucket> mBuckets;

public:
  DataStructure() = default;
  ~DataStructure() = default;

  // Returns the total number of items stored in the data structure.
  int GetItemsNumber() const;

  // Searches for an item by its ID string (e.g., "Cafe Noir").
  // Returns a pointer to the item if found, or nullptr if not found.
  // Note: pID is the item identifier string, not a pointer ID.
  Item *GetItem(char *pID) const;

  // Adds an item to the data structure.
  // Throws std::runtime_error if the ID is invalid or item already exists.
  void operator+=(Item& item);

  // Removes an item by its ID string.
  // Throws std::runtime_error if the ID is invalid or item not found.
  // Note: pID is the item identifier string, not a pointer ID.
  void operator-=(char *pID);

  // Prints all items in the data structure to the output stream.
  friend std::ostream &operator<<(std::ostream &ostr, const DataStructure &str);
};
