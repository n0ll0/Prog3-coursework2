#pragma once

#include "Item.h"

#include <array>
#include <forward_list>
#include <map>
#include <ostream>

class DataStructure
{
private:
  using Bucket = std::array<std::forward_list<Item>, 26>;
  std::map<char, Bucket> mBuckets;

public:
  DataStructure() = default;
  ~DataStructure() = default;

  int GetItemsNumber() const;
  Item *GetItem(char *pID) const;
  void operator+=(Item& item);
  void operator-=(char *pID);
  friend std::ostream &operator<<(std::ostream &ostr, const DataStructure &str);
};
