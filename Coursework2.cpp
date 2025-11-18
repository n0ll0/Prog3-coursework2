#include "DataProvider.h"
#include "Item.h"

#include <cstring>
#include <stdexcept>

namespace {
  ITEM1* fetchItem(char* pID) {
    ITEM1* temp = static_cast<ITEM1*>(GetItem(1, pID));
    if (!temp || !temp->pID) {
      throw std::runtime_error("Failed to retrieve item from provider");
      }
    return temp;
    }

  char* duplicateString(const char* source) {
    if (!source) {
      return nullptr;
      }
    const std::size_t len = std::strlen(source) + 1;
    char* copy = new char[len];
    strcpy_s(copy, len, source);
    return copy;
    }
  } // namespace

Item::Item() {
  ITEM1* temp = fetchItem(nullptr);
  pID = duplicateString(temp->pID);
  Code = temp->Code;
  pTime = duplicateString(temp->pTime);
  pNext = nullptr;
  }

Item::Item(char* pIDValue) {
  ITEM1* temp = fetchItem(pIDValue);
  pID = duplicateString(temp->pID);
  Code = temp->Code;
  pTime = duplicateString(temp->pTime);
  pNext = nullptr;
  }

Item::~Item() {
  delete[] pID;
  delete[] pTime;
  pNext = nullptr;
  }

Item::Item(const Item& orig) {
  pID = duplicateString(orig.pID);
  Code = orig.Code;
  pTime = duplicateString(orig.pTime);
  pNext = nullptr;
  }

Item& Item::operator=(const Item& right) {
  if (this != &right) {
    char* newID = duplicateString(right.pID);
    char* newTime = duplicateString(right.pTime);

    delete[] pID;
    delete[] pTime;

    pID = newID;
    pTime = newTime;
    Code = right.Code;
    pNext = nullptr;
    }
  return *this;
  }

bool Item::operator==(const Item& other) const {
  if (!pID || !other.pID) {
    return pID == other.pID;
  }
  return std::strcmp(pID, other.pID) == 0;
  }

std::ostream& operator<<(std::ostream& ostr, const Item& it) {
  if (it.pID) {
    ostr << it.pID;
  } else {
    ostr << "(null)";
  }
  return ostr;
  }
