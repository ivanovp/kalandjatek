/*
 * File:        inv.h
 * Purpose:     CInventory class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/26
 * Last modify: 2008-08-25 16:13:40 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file inv.h
 * \brief CInventory class header.
 * Used for drawing up an inventory, look, pick up or drop something.
 */

#ifndef __INCLUDE_INV_H
#define __INCLUDE_INV_H

#include <string>
#include <map>
#include <iostream>
#include <string>
#include "lang.h"

/// Struct for drawing up an inventory.
typedef struct 
{
    /// Number of this kind of items.
    int counter;
    /// 0: common noun (k�zn�v), 1: proper noun (tulajdonn�v).
    int noun;
    /// Price of one item.
    float price;
    /// Wight of one item.
    float weight;
    /// Plural of name.
    std::string plural;
    /// Additional information.
    std::string wearedon;
} TInventory;

/// Map for TInventory
typedef std::map<std::string, TInventory, CRuntimeStringCmp> CInventoryMap;
/// Iterator for CInventoryMap
typedef CInventoryMap::iterator CInventoryMapIt;

/**
 * Used for drawing up an inventory, look, pick up or drop something.
 */
class CInventory
{
private:
    std::ostringstream *os;
    /// Database for creating list of things.
    CInventoryMap *inventory_map;
    
public:
    /**
     * Constructor with setting the inventory map.
     * @param inventory_map Inventory map to process.
     */
    explicit CInventory (CInventoryMap &inventory_map);
    /// Constructor.
    CInventory ();
    /// Destructor.
    ~CInventory ();

    /**
     * Setting inventory map.
     * \param inventory_map .
     */
    void set_inventory_map (CInventoryMap &inventory_map);

    /**
     * Creates the list of things.
     * \return List of things. Example: "two orcs, one sword and three bread"
     */
    std::string write ();

    friend std::ostream& operator<< (std::ostream &os, CInventory &inv);
};
    
std::ostream& operator<< (std::ostream &os, CInventory &inv);

#endif // __INCLUDE_INV_H
