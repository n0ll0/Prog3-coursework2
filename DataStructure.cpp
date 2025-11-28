#include "DataStructure.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <stdexcept>

// =============================================================================
// This file implements a two-level hash-like data structure for storing Items.
//
// The storage works like this:
// - Items are indexed by their ID string, which has the format "FirstWord SecondWord"
// - The first level is a map keyed by the first letter of the first word (A-Z)
// - The second level is an array of 26 linked lists, indexed by the first letter
//   of the second word (A=0, B=1, ... Z=25)
//
// Example: "Cafe Noir" would be stored at:
//   mBuckets['C'][13]  (C for "Cafe", 13 for 'N' in "Noir")
// =============================================================================

namespace {

// Constants for valid uppercase letter range (A-Z)
constexpr char FIRST_VALID_LETTER = 'A';
constexpr char LAST_VALID_LETTER = 'Z';
constexpr char WORD_SEPARATOR = ' ';

// Holds the parsed components of an item identifier.
// The identifier format is "FirstWord SecondWord" (e.g., "Cafe Noir").
struct ParsedItemIdentifier {
  char firstWordInitial{};       // First letter of the first word (e.g., 'C' for "Cafe")
  std::size_t secondWordIndex{}; // Index derived from first letter of second word (0-25)
};

// Attempts to parse an item identifier string into its bucket keys.
// Returns true if parsing succeeded, false if the identifier is invalid.
// Valid format: "FirstWord SecondWord" where both words start with A-Z.
bool tryParseItemIdentifier(const char* itemIdentifier, ParsedItemIdentifier& parsedResult) {
  if (!itemIdentifier || itemIdentifier[0] == '\0') {
    return false;
  }

  const char firstWordInitial = itemIdentifier[0];
  if (firstWordInitial < FIRST_VALID_LETTER || firstWordInitial > LAST_VALID_LETTER) {
    return false;
  }

  const char* spacePosition = std::strchr(itemIdentifier, WORD_SEPARATOR);
  if (!spacePosition || !spacePosition[1]) {
    return false;
  }

  const char secondWordInitial = spacePosition[1];
  if (secondWordInitial < FIRST_VALID_LETTER || secondWordInitial > LAST_VALID_LETTER) {
    return false;
  }

  parsedResult.firstWordInitial = firstWordInitial;
  parsedResult.secondWordIndex = static_cast<std::size_t>(secondWordInitial - FIRST_VALID_LETTER);
  return true;
}

} // namespace

// Returns the total number of items stored in the data structure.
int DataStructure::GetItemsNumber() const {
  int totalCount = 0;
  for (const auto& bucketEntry : mBuckets) {
    for (const auto& itemList : bucketEntry.second) {
      totalCount += static_cast<int>(std::distance(itemList.begin(), itemList.end()));
    }
  }
  return totalCount;
}

// Searches for an item by its identifier string.
// Returns a pointer to the item if found, or nullptr if not found.
Item* DataStructure::GetItem(char* itemIdentifier) const {
  ParsedItemIdentifier parsedIdentifier;
  if (!tryParseItemIdentifier(itemIdentifier, parsedIdentifier)) {
    return nullptr;
  }

  const auto bucketIterator = mBuckets.find(parsedIdentifier.firstWordInitial);
  if (bucketIterator == mBuckets.end()) {
    return nullptr;
  }

  const auto& itemList = bucketIterator->second[parsedIdentifier.secondWordIndex];
  const auto foundItem =
      std::find_if(itemList.begin(), itemList.end(), [itemIdentifier](const Item& candidateItem) {
        return std::strcmp(candidateItem.GetID(), itemIdentifier) == 0;
      });

  if (foundItem == itemList.end()) {
    return nullptr;
  }

  return const_cast<Item*>(&(*foundItem));
}

// Adds an item to the data structure.
// Throws an exception if the item's ID is invalid or if an item with the same ID already exists.
void DataStructure::operator+=(Item& itemToAdd) {
  ParsedItemIdentifier parsedIdentifier;
  if (!tryParseItemIdentifier(itemToAdd.GetID(), parsedIdentifier)) {
    throw std::runtime_error("Invalid ID");
  }

  auto& itemList = mBuckets[parsedIdentifier.firstWordInitial][parsedIdentifier.secondWordIndex];
  const auto duplicateItem =
      std::find_if(itemList.begin(), itemList.end(), [&itemToAdd](const Item& existingItem) {
        return std::strcmp(existingItem.GetID(), itemToAdd.GetID()) == 0;
      });

  if (duplicateItem != itemList.end()) {
    throw std::runtime_error("Item already exists");
  }

  itemList.push_front(itemToAdd);
}

// Removes an item from the data structure by its identifier.
// Throws an exception if the item is not found or the ID is invalid.
void DataStructure::operator-=(char* itemIdentifier) {
  // Parse the identifier to extract the bucket keys
  ParsedItemIdentifier parsedIdentifier;
  if (!tryParseItemIdentifier(itemIdentifier, parsedIdentifier)) {
    throw std::runtime_error("Invalid ID");
  }

  // Find the bucket corresponding to the first word's initial letter
  auto bucketIterator = mBuckets.find(parsedIdentifier.firstWordInitial);
  if (bucketIterator == mBuckets.end()) {
    throw std::runtime_error("Item not found");
  }

  // Access the linked list for the second word's initial letter
  auto& itemList = bucketIterator->second[parsedIdentifier.secondWordIndex];
  auto previousIterator = itemList.before_begin();
  for (auto currentIterator = itemList.begin(); currentIterator != itemList.end(); ++currentIterator) {
    // Check if the current item matches the identifier we're looking for
    if (std::strcmp(currentIterator->GetID(), itemIdentifier) == 0) {
      // Remove the item from the linked list
      itemList.erase_after(previousIterator);

      // Check if the entire bucket (all 26 lists) is now empty
      if (itemList.empty()) {
        const bool isBucketCompletelyEmpty = std::all_of(
            bucketIterator->second.begin(), bucketIterator->second.end(),
            [](const auto& candidateList) { return candidateList.empty(); });
        // Note: The bucket could be removed from the map here if empty,
        // but this optimization is not implemented.
        (void)isBucketCompletelyEmpty; // Suppress unused variable warning
      }
      return;
    }
    ++previousIterator;
  }

  // If we reach here, no item with the given identifier was found
  throw std::runtime_error("Item not found");
}

// Stream output operator: prints all items in the data structure, one per line.
std::ostream& operator<<(std::ostream& outputStream, const DataStructure& dataStructure) {
  for (const auto& bucketEntry : dataStructure.mBuckets) {
    for (const auto& itemList : bucketEntry.second) {
      std::for_each(itemList.begin(), itemList.end(),
                    [&outputStream](const Item& currentItem) { outputStream << currentItem << std::endl; });
    }
  }
  return outputStream;
}
