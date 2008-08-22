/*
 * File:        cfg.h
 * Purpose:     CConfig class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/01/13
 * Last modify: 2008-08-21 10:40:15 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file cfg.h
 * \brief CConfig class header.
 */

#ifndef __INCLUDE_CFG_H
#define __INCLUDE_CFG_H

#include <string>
#include <iostream>
#include <fstream>
#include "regex.h"

/**
 * Configuration file reader class.
 *
 * With this class you can process simple configuration files like this:
 * <pre>
[section]
sStringVariable = "string"
sLongStringVariable = """
Long string.
Very long.
"""
iIntegerVariable = 100
fFloatVariable = 3.14
 *  </pre>
 *  Example:
 * <pre>
CConfig cfg;
if (!cfg.open ("foo.cfg"))
{
    return false;
}

while (cfg.ok ())
{
    if (cfg.process ())
    {
        std::string section = cfg.get_section ();
        // ...
    }
    std::string var, val;
    var = cfg.get_variable ();
    val = cfg.get_value ();
    // ...
}
 * </pre>
 */
class CConfig
{
private:
    std::string filename;       ///< Filename of config file.

    int line_counter;           ///< Actual line number of config file.
    std::string section;        ///< Section's name, format: [section].
    std::string variable;       ///< Variable's name, format: variable = value.
    std::string value;          ///< Value of variable, format: variable = value.
    CRegEx section_regex,
           number_regex,
           string_regex,
           longstring_regex,
           longstringend_regex;

    /// Returns true if c is space or tab character.
    bool iswhitespace (char c);
    /// Returns true if c is '#' or ';'.
    bool iscomment (char c);
    /// Cut out comments and whitespaces.
    std::string strip_comment (const std::string& s);
    
public:
    /// File stream for config file.
    std::ifstream infile;

    CConfig (const std::string& filename);
    CConfig ();
    ~CConfig ();

    /**
     * Opens a config file.
     * \param filename Filename of config file.
     * \return True if open is successful.
     */
    bool open (const std::string& filename);
    /**
     * Close opened config file.
     * \return True if close is successful
     */
    bool close ();

    /// \return Current section number.
    const std::string& get_section ();
    /// \return Variable's name.
    const std::string& get_variable ();
    /// \return Variable's value.
    const std::string& get_value ();

    /// \return Current line number.
    int get_line_counter () { return line_counter; };
    
    /**
     * You can get variable and value after each call of 'process'. 
     * \return True if new section found. 
     */
    bool process ();
    /**
     * Checks end of file flag.
     * \return True if not end of file.
     */
    bool ok ();
};

#endif // __INCLUDE_CFG_H
