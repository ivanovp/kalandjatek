/*
 * File:        cfg.cc
 * Purpose:     CConfig class
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/01/13
 * Last modify: 2008-08-21 10:40:10 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */

#include "cfg.h"
#include "debug.h"

CConfig::CConfig (const std::string& filename)
{
    CConfig::CConfig ();
    open (filename);
}

CConfig::CConfig ()
{
    section_regex.Compile ("\\[(.*)\\]");
    number_regex.Compile ("([if]\\w+)\\s*[=]\\s*([-+.,0-9]+)");
    string_regex.Compile ("(s\\w+)\\s*=\\s*\"(.*)\"");
    longstring_regex.Compile ("(s\\w+)\\s*=\\s*\"\"\"(.*)");
    longstringend_regex.Compile ("^(.*)\"\"\"");
}

CConfig::~CConfig ()
{
}

bool CConfig::open (const std::string& filename)
{
    line_counter = 0;
    //infile.exceptions (std::ios::failbit | std::ios::badbit);
    this->filename = filename;
    infile.open (filename.c_str ());
    if (!infile)
    {
        std::ostringstream os;
        os << __INFO__ << "Can't open " << filename << " file for reading!";
        Log.error (os.str ());
        return false;
    }
    return true;
}

bool CConfig::close ()
{
    infile.close ();
    return true;
}
    
const std::string& CConfig::get_section ()
{
    return section;
}

const std::string& CConfig::get_variable ()
{
    return variable;
}

const std::string& CConfig::get_value ()
{
    return value;
}

// for internal use only
bool CConfig::iswhitespace (char c)
{
    if (c == ' ' || c == '\t') 
        return true;
    return false;
}

// for internal use only
bool CConfig::iscomment (char c)
{
    if (c == ';' || c == '#') 
        return true;
    return false;
}

// for internal use only
// cut out comments and whitespaces
std::string CConfig::strip_comment (const std::string& s)
{
    std::string s2;
    // mode:
    // 0 1       2   3 4     
    //   variable  =   value
    int mode = 0;
    for (unsigned int i = 0; i < s.length (); i++)
    {
        //std::cout << mode << s[i];
        switch (mode)
            {
            case 0:
                if (iscomment (s[i])) return "";
                if (!iswhitespace (s[i])) 
                {
                    s2 += s[i];
                    mode++;
                }
                break;
            case 1:
                if (iscomment (s[i])) return s2; // TODO error
                if (iswhitespace (s[i])) 
                {
                    mode++;
                    break;
                }
                if (s[i] == '=') mode = 4;
                s2 += s[i];
                break;
            case 2:
                if (s[i] == '=') 
                {
                    s2 += s[i];
                    mode++;
                }
                break;
            case 3:
                if (iswhitespace (s[i]))
                    break;
                else
                    mode = 4;
            case 4:
                if (s[i] == '"') mode = 5;
                if (iscomment (s[i]) || iswhitespace (s[i])) return s2;
                s2 += s[i];
                break;
            case 5:
                s2 += s[i];
                if (s[i - 1] != '\\' && s[i] == '"')
                {
                    if (s[i + 1] == '"') s2 += s[++i]; // long string
                    return s2;
                }
                break;
            default:
                break;
        }
    }
    return s2; // hmm
}

// reads lines and returns with true if new section found
bool CConfig::process ()
{
    bool ok = true;
    bool new_section = false;
    int longstring_mode = 0;

    variable.clear ();
    value.clear ();

    while (infile && ok)
    {
        char buf[1024];  // maximum width of line!
        infile.getline (buf, sizeof (buf));
        line_counter++;

        std::string orig_line, line;
        orig_line = line = buf;
        //std::cout << "line: [" << line << "]" << std::endl;

        if (!longstring_mode) 
        {
            line = strip_comment (line);
            //std::cout << "line without comment: [" << line << "]" << std::endl;
            //std::cout << std::endl;
        }
            
        if (longstring_mode)
        {
            // in longstring mode we permit comment char '#', ';' in first position
            if (line[0] == '#' || line[0] == ';')
            {
                continue;
            }
            if (longstringend_regex.Matches (line))
            {
                //std::cout << __INFO__ << "endlongstring: " << variable << " = [" << value << "]" << std::endl;
                longstring_mode = 0;
                ok = false;
                continue;
            }
            else
            {
                // adding a space to end of line when not present
                if (!iswhitespace (line[line.length () - 1]))
                {
                    line += " ";
                }
                value += line;
                continue;
            }
        }

        if (longstring_regex.Matches (line))
        {
            variable = longstring_regex.GetMatch (line, 1);
            value.clear ();
            //std::cout << __INFO__ << "longstring: " << variable << " = [" << value << "]" << std::endl;
            longstring_mode = 1;
            continue;
        }
        
        if (string_regex.Matches (line))
        {
            variable = string_regex.GetMatch (line, 1);
            value = string_regex.GetMatch (line, 2);
            //std::cout << __INFO__ << "string: " << variable << " = [" << value << "]" << std::endl;
            ok = false;
            continue;
        }
        
        if (number_regex.Matches (line))
        {
            variable = number_regex.GetMatch (line, 1);
            value = number_regex.GetMatch (line, 2);
            //std::cout << __INFO__ << "number: " << variable << " = [" << value << "]" << std::endl;
            ok = false;
            continue;
        }
        
        if (section_regex.Matches (line))
        {
            section = section_regex.GetMatch (line, 1);
            //std::cout << __INFO__ << "section: " << section << std::endl;
            new_section = true;
            continue;
        }
        
        if (line.length () > 0)
        {
            std::ostringstream os;
            os << __INFO__ << "Invalid line in configuration file. " << filename << ":" << line_counter << ": " << orig_line << " (" << line << ")";
            Log.error (os.str ());
        }
        //std::cout << std::endl;
    }
    return new_section;
}

bool CConfig::ok ()
{
    return infile;
}
