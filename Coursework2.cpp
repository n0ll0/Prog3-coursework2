#include "DataProvider.h"
#include "Item.h"

#include <cstring>
#include <stdexcept>

namespace {
  // Fetches an item from the external data provider library.
  // If itemIdentifier is nullptr, a random item is returned.
  ITEM1* fetchItemFromProvider(char* itemIdentifier) {
    ITEM1* fetchedItem = static_cast<ITEM1*>(GetItem(1, itemIdentifier));
    if (!fetchedItem || !fetchedItem->pID) {
      throw std::runtime_error("Failed to retrieve item from provider");
      }
    return fetchedItem;
    }

  // Creates a heap-allocated copy of a C-string.
  // Returns nullptr if source is nullptr.
  char* duplicateString(const char* source) {
    if (!source) {
      return nullptr;
      }
    const std::size_t length = std::strlen(source) + 1;
    char* copy = new char[length];
    strcpy_s(copy, length, source);
    return copy;
    }
  } // namespace

// Default constructor: fetches a random item from the data provider.
Item::Item() {
  ITEM1* fetchedItem = fetchItemFromProvider(nullptr);
  pID = duplicateString(fetchedItem->pID);
  Code = fetchedItem->Code;
  pTime = duplicateString(fetchedItem->pTime);
  pNext = nullptr;
  }

// Parameterized constructor: fetches an item with the given identifier.
// If itemIdentifier is nullptr, a random item is fetched.
Item::Item(char* itemIdentifier) {
  ITEM1* fetchedItem = fetchItemFromProvider(itemIdentifier);
  pID = duplicateString(fetchedItem->pID);
  Code = fetchedItem->Code;
  pTime = duplicateString(fetchedItem->pTime);
  pNext = nullptr;
  }

// Destructor: frees the dynamically allocated ID and time strings.
Item::~Item() {
  delete[] pID;
  delete[] pTime;
  pNext = nullptr;
  }

// Copy constructor: creates a deep copy of the sourceItem.
Item::Item(const Item& sourceItem) {
  pID = duplicateString(sourceItem.pID);
  Code = sourceItem.Code;
  pTime = duplicateString(sourceItem.pTime);
  pNext = nullptr;
  }

// Assignment operator: performs a deep copy of otherItem.
// Uses copy-and-swap idiom to ensure exception safety.
Item& Item::operator=(const Item& otherItem) {
  if (this != &otherItem) {
    char* newID = duplicateString(otherItem.pID);
    char* newTime = duplicateString(otherItem.pTime);

    delete[] pID;
    delete[] pTime;

    pID = newID;
    pTime = newTime;
    Code = otherItem.Code;
    pNext = nullptr;
    }
  return *this;
  }

// Equality operator: two items are equal if they have the same ID string.
bool Item::operator==(const Item& other) const {
  if (!pID || !other.pID) {
    return pID == other.pID;
  }
  return std::strcmp(pID, other.pID) == 0;
  }

// Stream output operator: prints the item's ID, or "(null)" if the ID is empty.
std::ostream& operator<<(std::ostream& outputStream, const Item& itemToOutput) {
  if (itemToOutput.pID) {
    outputStream << itemToOutput.pID;
  } else {
    outputStream << "(null)";
  }
  return outputStream;
  }
