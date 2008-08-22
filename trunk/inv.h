/*
 * File:        inv.h
 * Purpose:     CInventory class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/26
 * Last modify: 2008-08-21 10:41:47 ivanovp {Time-stamp}
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
    /// 0: common noun (köznév), 1: proper noun (tulajdonnév).
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
    std::ostream *os;
    
public:
    /// Database for creating list of things.
    CInventoryMap inventory_map;

    /**
     * Constructor with setting output stream.
     * \param os Output stream.
     */
    CInventory (std::ostream *os);
    /// Constructor.
    CInventory ();
    /// Destructor.
    ~CInventory ();

    /**
     * Setting output stream.
     * \param os Output stream.
     * \note If you use stream then write () returns empty string.
     */
    void set_stream (std::ostream *os);

    /**
     * Creates the list of things.
     * \return List of things if stream not initialized.
     * \see set_stream
     */
    std::string write ();
};

#endif // __INCLUDE_INV_H
