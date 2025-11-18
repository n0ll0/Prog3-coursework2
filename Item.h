#pragma once

#include "Items.h"

#include <ostream>

// Coursework uses ITEM1; adjust to other ITEMn variants if required.
class Item : public ITEM1
{
public:
    Item();
    explicit Item(char *pID);
    ~Item();
    Item(const Item& orig);
    Item& operator=(const Item& right);
    bool operator==(const Item& other) const;
    friend std::ostream &operator<<(std::ostream& ostr, const Item &it);
    char* GetID() const { return pID; }
};