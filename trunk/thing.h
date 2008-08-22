/*
 * File:        thing.h
 * Purpose:     CThing class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/01/13
 * Last modify: 2008-08-22 09:43:15 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file thing.h
 * \brief CThing class header.
 */

#ifndef __INCLUDE_THING_H
#define __INCLUDE_THING_H

#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>

#include <log4cpp/Category.hh>
#include "regex.h"
#include "split.h" // CStringVector

class CThing;
typedef std::list<CThing*> CThingList;                  ///< Thing list type
typedef CThingList::iterator CThingListIt;              ///< Thing list iterator type

typedef std::map<std::string,std::string> CThingSParam; ///< String parameters type
typedef CThingSParam::iterator CThingSParamIt;          ///< String parameters iterator type
typedef std::map<std::string,int> CThingIParam;         ///< Integer parameters type
typedef CThingIParam::iterator CThingIParamIt;          ///< Integer parameters iterator type
typedef std::map<std::string,float> CThingFParam;       ///< Float parameters type
typedef CThingFParam::iterator CThingFParamIt;          ///< Float parameters iterator type

/**
 * Base object of everything
 */
class CThing
{
private:
    signed long sn;             ///< Unique serial number.
    std::string id;             ///< Internal identifier (example: tree).
    
    void init ();

public:
    static const char* K_NAME;
    static const char* K_DESCR;
    static const char* K_ALTNAME;
    static const char* K_RAGOK;
    static const char* K_PLURAL;
    static const char* K_CHILDS;
    static const char* K_BODY;
    static const char* K_STRENGTH;
    static const char* K_DEXTERITY;
    static const char* K_PERCEPTION;
    static const char* K_INTELLIGENCE;
    static const char* K_WILLPOWER;
    static const char* K_CHARISMA;
    static const char* K_BDY;
    static const char* K_STR;
    static const char* K_DEX;
    static const char* K_PER;
    static const char* K_INT;
    static const char* K_WIL;
    static const char* K_CHA;
    static const char* K_HP;
    static const char* K_MP;
    static const char* K_ATTACK;
    static const char* K_DEFENSE;

    /// Last used serial number.
    static signed long last_sn;
    /// Used to register the things in our universe.
    static CThingList global_thinglist;
    /// Used to register the players (and spectators).
    static CThingList global_spectator_thinglist;
    /// Name of statistics. Example: strength, hit points, etc.
    static CStringVector stat_name_vector;
    /// Keyword of statistics. Example: iStrength, iHP, etc.
    static CStringVector stat_keyword_vector;
    /// Hungarian endings and others for CThing::get_name ().
    enum 
    {
        M_NORMAL = 0,               ///< Get default name.
        M_RAG_VAL = 1,              ///< Get name with hungarian ending -val/-vel
        M_RAG_T = 2,                ///< Get name with hungarian ending -t 
        M_RAG_RA = 3,               ///< Get name with hungarian ending -ra/-re
        M_RAG_BOL = 4,              ///< Get name with hungarian ending -b�l/-b�l
        M_RAG_ROL = 5,              ///< Get name with hungarian ending -r�l/-r�l
        M_RAG_TOL = 6,              ///< Get name with hungarian ending -t�l/-t�l
        M_RAG_BA = 7,               ///< Get name with hungarian ending -ba/-be
        M_CAPITAL_FIRST = 8,        ///< Make first alphabet to capital.
        M_ARTICLE = 16,             ///< Puts a definite article to the name. (Hat�rozott n�vel�t rak ki.)
        M_IARTICLE = 32             ///< Puts an indefinite article to the name. (Hat�rozatlan n�vel�t rak ki.)
    };

    /// Statistic types.
    typedef enum
    {
        S_BODY = 0,         S_BDY = S_BODY,
        S_STRENGTH = 1,     S_STR = S_STRENGTH,
        S_DEXTERITY = 2,    S_DEX = S_DEXTERITY,
        S_PERCEPTION = 3,   S_PER = S_PERCEPTION,
        S_INTELLIGENCE = 4, S_INT = S_INTELLIGENCE,
        S_WILLPOWER = 5,    S_WIL = S_WILLPOWER,
        S_CHARISMA = 6,     S_CHA = S_CHARISMA,
        S_CURR_HP = 7,      S_HP = S_CURR_HP,   // HP modifier of an item
        S_MAX_HP = 8,       
        S_CURR_MP = 9,      S_MP = S_CURR_MP,   // MP modifier of an item
        S_MAX_MP = 10,
        S_ATTACK = 11,
        S_DEFENSE = 12,
        S_STAT_NUMBER // used to count number of statistics
    } stat_t;
        
public:
    CThing *parent;             ///< Parent of this thing. NULL if not exists.
    CThingList childs;          ///< Childs of this thing.
    CThingSParam sparams;       ///< String parameters of this thing.
    CThingIParam iparams;       ///< integer parameters of this thing.
    CThingFParam fparams;       ///< Float parameters of this thing.

    /// Constructor.
    CThing ();
    /// Constructor.
    CThing (const std::string& id, const std::string& name = "", 
            const std::string& descr = "", CThing *parent = NULL);
    /// Copy constructor.
    CThing (CThing& thing);
    /// Equal sign operator.    
    virtual CThing& operator= (CThing& thing);

    /// Destructor.
    virtual ~CThing ();

    /// Copy childs from this thing to parameter.
    void copy_childs_to (CThing& thing);
    /// Copy childs of parameter to this thing.
    void copy_childs_from (CThing& thing);
    /// Move this thing to parameter thing.
    void move_to (CThing& thing);

    /**
     * After loading string, integer, float parameters must be call setup ().
     * This function find sChilds and copy them.
     */
    virtual void setup ();

    /**
     * Get serial number.
     * \return Unique serial number.
     */
    signed long get_sn ();

    /**
     * Set identifier.
     * \param id Identifier.
     */
    bool set_id (const std::string& id);

    /**
     * Get identifier.
     * \return Identifier.
     */
    std::string get_id ();

    /**
     * Set name of thing.
     * \param name Name.
     */
    bool set_name (const std::string& name);

    /**
     * Get name of thing.
     * \param mode Name mode options. Example: <code>thing->get_name (M_ARTICLE | M_CAPITAL_FIRST);</code>
     * \return Name.
     */
    std::string get_name (int mode = M_NORMAL);
    
    /**
     * Compare name of thing.
     * \param name The searched name.
     * \return True: if one of the name equal to parameter.
     */
    bool compare_name (const std::string& name);

    /**
     * Set short description of thing.
     * \param descr Description.
     */
    
    bool set_descr (const std::string& descr);
    /**
     * Get short description of thing.
     * \return Description.
     */
    std::string get_descr ();

    /**
     * Set string parameter.
     * \param variable First character of variable must be 's'.
     * \param value Value of variable.
     */
    bool set_sparam (const std::string& variable, const std::string& value);

    /**
     * Get string parameter.
     * \param variable First character of variable must be 's'.
     * \return Value of variable or null string if not found.
     */
    const std::string& get_sparam (const std::string& variable);
    
    /**
     * Set integer parameter.
     * \param variable First character of variable must be 'i'.
     * \param value Value of variable.
     */
    bool set_iparam (const std::string& variable, const int& value);

    /**
     * Get integer parameter.
     * \param variable First character of variable must be 'i'.
     * \return Value of variable or zero if not found.
     */
    int get_iparam (const std::string& variable);

    /**
     * Set float parameter.
     * \param variable First character of variable must be 'f'.
     * \param value Value of variable.
     */
    bool set_fparam (const std::string& variable, const float& value);

    /**
     * Get float parameter.
     * \param variable First character of variable must be 'f'.
     * \return Value of variable or zero if not found.
     */
    float get_fparam (const std::string& variable);

    /**
     * Get specified statistic of thing.
     */
    virtual int get_stat (int stat_type);
    
    /**
     * Draw the shape of thing.
     * \return The shape of thing (usually one char).
     */
    virtual std::string get_shape ();
    
    /**
     * Draw the color of thing.
     * \return The color of thing.
     */
    virtual int get_color ();

    /**
     * Calling periodically by parent.
     */
    virtual void do_something ();
    
    /**
     * Go through list of childs and call do_something()
     */
    void childs_do_something ();

    /**
     * Find thing in parent of thing childs.
     * \param id The identifier of thing
     * \param thinglist Find in the thinglist.
     * \return Pointer to thing or NULL if not found.
     */
    CThing* find (const std::string& id, CThingList& thinglist);

    /** 
     * Shows informations (only for debugging).
     */
    virtual std::string info (int verbose_level = 0);

    /**
     * Getting type of thing.
     * \return Return the type of this thing.
     */
    virtual std::string get_type () { return "thing"; };

    /**
     * Copy self.
     * \return Return the copy of this thing.
     */
    virtual CThing* copy () { return new CThing (*this); };
    
    int operator== (CThing &thing) { return (get_sn () == thing.get_sn ()); }
    int operator!= (CThing &thing) { return (get_sn () != thing.get_sn ()); }

    // uses info()
    friend std::ostream& operator<< (std::ostream &os, CThing &thing);
}; // CThing

#endif // __INCLUDE_THING_H