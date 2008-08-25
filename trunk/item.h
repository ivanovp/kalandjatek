/*
 * File:        item.h
 * Purpose:     CItem class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-25 11:10:56 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file item.h
 * \brief CItem class header.
 */

#ifndef __INCLUDE_ITEM_H
#define __INCLUDE_ITEM_H

#include "thing.h"

/**
 * Class for lifeless things, weapons, armors.
 */
class CItem: public CThing
{
private:
    void init ();

public:
    static const std::string ITEM; // for get_type()
    // CItem keywords
    static const std::string K_PRICE;
    static const std::string K_WEIGHT;
    static const std::string K_MOVABLE;
    static const std::string K_VISIBLE;
    static const std::string K_WEAREDON;
    // CItem keywords
    static const std::string K_TYPE;
    static const std::string K_RING;
    static const std::string K_BRACELET;
    static const std::string K_AMULET;
    static const std::string K_CLOAK;
    static const std::string K_PANTS;
    static const std::string K_FOOTWEAR;
    static const std::string K_GLOVE;
    static const std::string K_CAP;
    static const std::string K_TORSO;
    static const std::string K_ARMGUARDS;
    static const std::string K_SHINGUARDS;
    static const std::string K_ONE_HANDED;
    static const std::string K_TWO_HANDED;
    static const std::string K_CURRENCY;
    static const std::string K_KEY;

    static CThingList global_itemlist;

public:
    /// Constructor
    CItem (); 
    /// Constructor
    CItem (const std::string& id, const std::string& name = "", 
            const std::string& descr = "", CThing *parent = NULL);
    /// Copy constructor
    CItem (CThing& thing);
    /// Copy constructor
    CItem (CItem& thing);
    /// Destructor
    virtual ~CItem ();

    /**
     * Draw the shape of item.
     * \return The shape of item (usually one char).
     */
    virtual std::string get_shape ();
    
    /**
     * Draw the color of item.
     * \return The color of item.
     */
    virtual int get_color ();

    /**
     * Calling periodical by parent.
     */
    virtual void do_something ();

    /**
     * Getting price string of item.
     * \param price If you want get another price. -1 means get price of this item.
     * \return Return the price of this item or given price.
     */
    std::string get_price_str (float price = -1);

    /**
     * Copy self.
     * \return Return the copy of this item.
     */
    virtual CThing* copy () { return new CItem (*this); };
}; // CItem

#endif // __INCLUDE_ITEM_H
