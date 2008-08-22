/*
 * File:        map.h
 * Purpose:     CMap class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 13:51:58 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file map.h
 * \brief CMap class header.
 */

#ifndef __INCLUDE_MAP_H
#define __INCLUDE_MAP_H

#include "thing.h"

/**
 * Class for places where creatures can live.
 */
class CMap: public CThing
{
private:
    void init ();

public:
    static const std::string K_EXITS;
    static CThingList global_maplist;

public:
    /// Constructor
    CMap (); 
    /// Constructor
    CMap (const std::string& id, const std::string& name = "", 
            const std::string& descr = "", CThing *parent = NULL);
    /// Copy constructor
    CMap (CThing& thing);
    /// Copy constructor
    CMap (CMap& thing);
    /// Destructor
    virtual ~CMap ();

    /**
     * Draw the map. Not implemented yet. @todo Implement map draw()
     */
    virtual void draw ();
    
    /**
     * Calling periodical by parent.
     */
    virtual void do_something ();

    /**
     * Copy self.
     * \return Return the copy of this map.
     */
    virtual CThing* copy () { return new CMap (*this); };
}; // CMap

#endif // __INCLUDE_MAP_H
