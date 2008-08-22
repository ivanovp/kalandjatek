/*
 * File:        debug.h
 * Purpose:     Debugger util
 * Author:      Peter Ivanov
 * Modified by: 
 * Created:     2000/12/31
 * Last modify: 2008-08-21 10:41:34 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 1997-2005
 * Licence:     GPL
 */
/**
 * \file debug.h
 * \brief Debugger utilities.
 */

#ifndef __INCLUDE_DEBUG_H
#define __INCLUDE_DEBUG_H

// using liblog4cpp or iostream
#define USE_LOG4CPP     1

#ifdef USE_LOG4CPP

#include <log4cpp/Category.hh>
extern log4cpp::Category& Log;
//extern log4cpp::Category& Trace;

#else // USE_LOG4CPP

#include <iostream>

#endif // USE_LOG4CPP

//#define __INFO__        __FUNCTION__ << " [" << __FILE__ ":" << __LINE__ << "]: "
#define __INFO__        "[" << __FILE__ ":" << __LINE__ << "]: "

#ifdef __DEBUG__

#ifdef USE_LOG4CPP

#define LOG             Log
#define ENDL            log4cpp::CategoryStream::ENDLINE

#else // USE_LOG4CPP

#define LOG             std::cout
#define ENDL            std::endl

#endif // USE_LOG4CPP

#define PRINT(str)      LOG << __INFO__ << str << ENDL
#define PRINTVAL(value) LOG << __INFO__ << #value " = " << value << ENDL
#define PRINTSTR(str)   LOG << __INFO__ << #str " = [" << str << "]" << ENDL
#define PRINTCHR(chr)   LOG << __INFO__ << #chr " = " << chr << ENDL
#define PRINTNL         LOG << ENDL

#else   // __DEBUG__

#define PRINT(info)     
#define PRINTVAL(value) 
#define PRINTSTR(str)   
#define PRINTCHR(chr)   
#define PRINTNL 

#endif  // __DEBUG__
                                                                         
#endif  // __INCLUDE_DEBUG_H

/*
$Log$
*/
