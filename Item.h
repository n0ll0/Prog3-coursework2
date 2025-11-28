#pragma once

#include "Items.h"

#include <ostream>

// =============================================================================
// Item: Represents a single data item with an ID, code, and timestamp.
//
// This class inherits from ITEM1 (defined in Items.h), which provides:
//   - pID:   Pointer to a C-string containing the item's identifier
//            (format: "FirstWord SecondWord", e.g., "Cafe Noir")
//   - Code:  An unsigned long value associated with the item
//   - pTime: Pointer to a C-string containing timestamp information
//   - pNext: Pointer to the next item (used for linked list structures)
//
// The Item class adds proper C++ memory management (Rule of Three):
// constructors, destructor, copy constructor, and assignment operator
// that handle the dynamically allocated string members.
// =============================================================================

// Coursework uses ITEM1; adjust to other ITEMn variants if required.
class Item : public ITEM1
{
public:
    // Default constructor: fetches a random item from the data provider.
    Item();

    // Parameterized constructor: fetches an item with the given identifier.
    // If pID is nullptr, a random item is fetched.
    // Note: pID here refers to the item identifier string, not "pointer ID".
    explicit Item(char *pID);

    // Destructor: frees the dynamically allocated ID and time strings.
    ~Item();

    // Copy constructor: creates a deep copy of the source item.
    Item(const Item& orig);

    // Assignment operator: performs a deep copy from the right-hand side item.
    Item& operator=(const Item& right);

    // Equality operator: two items are equal if they have the same ID string.
    bool operator==(const Item& other) const;

    // Stream output operator: prints the item's ID to the output stream.
    friend std::ostream &operator<<(std::ostream& ostr, const Item &it);

    // Returns a pointer to the item's identifier string.
    char* GetID() const { return pID; }
};