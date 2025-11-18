#include "DataStructure.h"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <stdexcept>

namespace {
struct ParsedKey {
  char firstLetter{};
  std::size_t secondIndex{};
};

bool tryParseKey(const char* pID, ParsedKey& key) {
  if (!pID || pID[0] == '\0') {
    return false;
  }

  const char first = pID[0];
  if (first < 'A' || first > 'Z') {
    return false;
  }

  const char* space = std::strchr(pID, ' ');
  if (!space || !space[1]) {
    return false;
  }

  const char second = space[1];
  if (second < 'A' || second > 'Z') {
    return false;
  }

  key.firstLetter = first;
  key.secondIndex = static_cast<std::size_t>(second - 'A');
  return true;
}

} // namespace

int DataStructure::GetItemsNumber() const {
  int count = 0;
  for (const auto& entry : mBuckets) {
    for (const auto& list : entry.second) {
      count += static_cast<int>(std::distance(list.begin(), list.end()));
    }
  }
  return count;
}

Item* DataStructure::GetItem(char* pID) const {
  ParsedKey key;
  if (!tryParseKey(pID, key)) {
    return nullptr;
  }

  const auto outer = mBuckets.find(key.firstLetter);
  if (outer == mBuckets.end()) {
    return nullptr;
  }

  const auto& list = outer->second[key.secondIndex];
  const auto found =
      std::find_if(list.begin(), list.end(), [pID](const Item& item) {
        return std::strcmp(item.GetID(), pID) == 0;
      });

  if (found == list.end()) {
    return nullptr;
  }

  return const_cast<Item*>(&(*found));
}

void DataStructure::operator+=(Item& item) {
  ParsedKey key;
  if (!tryParseKey(item.GetID(), key)) {
    throw std::runtime_error("Invalid ID");
  }

  auto& list = mBuckets[key.firstLetter][key.secondIndex];
  const auto duplicate =
      std::find_if(list.begin(), list.end(), [&item](const Item& existing) {
        return std::strcmp(existing.GetID(), item.GetID()) == 0;
      });

  if (duplicate != list.end()) {
    throw std::runtime_error("Item already exists");
  }

  list.push_front(item);
}

void DataStructure::operator-=(char* pID) {
  ParsedKey key;
  if (!tryParseKey(pID, key)) {
    throw std::runtime_error("Invalid ID");
  }

  auto outer = mBuckets.find(key.firstLetter);
  if (outer == mBuckets.end()) {
    throw std::runtime_error("Item not found");
  }

  auto& list = outer->second[key.secondIndex];
  auto prev = list.before_begin();
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (std::strcmp(it->GetID(), pID) == 0) {
      list.erase_after(prev);

      if (list.empty()) {
        const bool emptyBucket = std::all_of(
            outer->second.begin(), outer->second.end(),
            [](const auto& candidate) { return candidate.empty(); });
      }
      return;
    }
    ++prev;
  }

  throw std::runtime_error("Item not found");
}

std::ostream& operator<<(std::ostream& ostr, const DataStructure& str) {
  for (const auto& entry : str.mBuckets) {
    for (const auto& list : entry.second) {
      std::for_each(list.begin(), list.end(),
                    [&ostr](const Item& item) { ostr << item << std::endl; });
    }
  }
  return ostr;
}
