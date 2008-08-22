/*
 * File:        lang.cc
 * Purpose:     Language specific code
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/16
 * Last modify: 2008-08-21 10:42:39 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "lang.h"

// Upper case string
std::string upper (const std::string& s)
{
    CTrans tr;
    std::string S (s);
    tr ("a-záéíóöõúüû", "A-ZÁÉÍÓÖÕÚÜÛ", S);
    return S;
}

// Upper case first char of string
std::string Upper (const std::string& s)
{
    CTrans tr;
    std::string S;
    S = s[0];
    tr ("a-záéíóöõúüû", "A-ZÁÉÍÓÖÕÚÜÛ", S);
    S += s.substr (1, s.size () - 1);
    return S;
}

// Lower case string
std::string lower (const std::string& s)
{
    CTrans tr;
    std::string S (s);
    tr ("A-ZÁÉÍÓÖÕÚÜÛ", "a-záéíóöõúüû", S);
    return S;
}

// Lower case first char of string
std::string Lower (const std::string& s)
{
    CTrans tr;
    std::string S;
    S = s[0];
    tr ("A-ZÁÉÍÓÖÕÚÜÛ", "a-záéíóöõúüû", S);
    S += s.substr (1, s.size () - 1);
    return S;
}

