/*
 * File:        dice.h
 * Purpose:     CDice class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2008-08-22
 * Last modify: 2008-08-23 11:41:40 ivanovp {Time-stamp}
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

    int roll (const std::string& type);

    int operator() (const std::string& type) { return roll (type); }
};

extern CDice dice;

#endif // __INCLUDE_DICE_H

