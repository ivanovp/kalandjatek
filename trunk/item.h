/*
 * File:        item.h
 * Purpose:     CItem class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 09:46:17 ivanovp {Time-stamp}
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
    // CItem keywords
    static const char* K_PRICE;
    static const char* K_WEIGHT;
    static const char* K_MOVABLE;
    static const char* K_VISIBLE;
    static const char* K_WEAREDON;

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
    virtual std::string get_price_str (float price = -1);

    /**
     * Getting type of thing.
     * \return Return the type of this thing.
     */
    virtual std::string get_type () { return "item"; };

    /**
     * Copy self.
     * \return Return the copy of this item.
     */
    virtual CThing* copy () { return new CItem (*this); };
}; // CItem

#endif // __INCLUDE_ITEM_H