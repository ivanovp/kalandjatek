/*
 * File:        lang.h
 * Purpose:     Language selector header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/16
 * Last modify: 2008-08-21 10:42:42 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file lang.h
 * \brief Language specific classes and function.
 */

#ifndef __INCLUDE_LANG_H
#define __INCLUDE_LANG_H

#include "trans.h"

#define ENG     0       ///< English language.
#define HUN     1       ///< Hungarian language.

#ifndef LANG
/// Setting default language.
#define LANG    HUN
#endif

/**
 * Language specific string comparer.
 * 
 * Example:
 * <pre>
typedef std::map<std::string, CFunctor*, CRuntimeStringCmp> CFunctorMap;
std::map<std::string, int, CRuntimeStringCmp> my_map;
 * </pre>
 */
class CRuntimeStringCmp 
{
private:
#if (LANG == HUN)
    CTrans tr;
#endif
      
public:  
    CRuntimeStringCmp ()
    {
#if (LANG == HUN)
        tr.set_lists ("·ÈÌÛˆı˙¸˚¡…Õ”÷’⁄‹€", 
                      "aeiooouuuAEIOOOUUU");
#endif
    }
    
    // the comparison
    bool operator() (const std::string& s1, const std::string& s2)
    {
#if (LANG == HUN)
        std::string S1 = s1, S2 = s2;
        tr (S1);
        tr (S2);
        return S1 < S2;
#endif
#if (LANG == ENG)
        return s1 < s2;
#endif
    }
};

/// Upper case string
std::string upper (const std::string& s);
/// Upper case first char of string
std::string Upper (const std::string& s);
/// Lower case string
std::string lower (const std::string& s);
/// Lower case first char of string
std::string Lower (const std::string& s);
#endif // __INCLUDE_LANG_H
