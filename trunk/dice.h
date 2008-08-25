/*
 * File:        dice.h
 * Purpose:     CDice class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2008-08-22
 * Last modify: 2008-08-25 14:32:57 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file dice.h
 * \brief CDice class header.
 */

#ifndef __INCLUDE_DICE_H
#define __INCLUDE_DICE_H

#include <string>
#include <iostream>

/**
 * Class to roll a dice
 */
class CDice
{
private:
    
public:
    /// Constructor.
    CDice ();
    
    /// Destructor.
    ~CDice ();

    /**
     * Roll the dice!
     *
     * @param type Dice type. Format: "min-max". For a cube dice: "1-6".
     * @return The result of rolling.
     */
    int roll (const std::string& type);

    /**
     * Same as function roll. @see roll()
     */
    int operator() (const std::string& type) { return roll (type); }
};

extern CDice dice;

#endif // __INCLUDE_DICE_H

