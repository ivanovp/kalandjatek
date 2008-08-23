/*
 * File:        dice.cc
 * Purpose:     CDice class implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2008-08-22
 * Last modify: 2008-08-23 14:14:08 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

#include "dice.h"
#include "regex.h"
#include "debug.h"

CDice dice;

CDice::CDice ()
{
    srand (static_cast<unsigned> (time (0)));
}

CDice::~CDice ()
{
}

int CDice::roll (const std::string& type)
{
    int x = 0;
    CRegEx regex ("(\\d+)-(\\d+)");
    if (regex.Matches (type))
    {
        std::string s1, s2;
        s1 = regex.GetMatch (type, 1);
        s2 = regex.GetMatch (type, 2);
        int a, b;
        a = std::strtol (s1.c_str (), NULL, 10);
        b = std::strtol (s2.c_str (), NULL, 10);
        x = a + static_cast<int> (static_cast<float> (b - a) * rand () / (RAND_MAX + 1.0));
        std::cout << "dice: " << std::left << std::setw (10) << type 
            << " " << std::setw (5) << a << "-" << std::setw (5) << b << " x = " << std::setw (5) << x << std::endl;
    }
    else
    {
        std::ostringstream os;
        os << __INFO__ << "Invalid dice type: [" << type << "]";
        Log.warn (os.str ());
    }
    return x;
}
