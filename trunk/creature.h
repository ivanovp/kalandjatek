/*
 * File:        creature.h
 * Purpose:     CCreature class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-22 14:31:11 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file creature.h
 * \brief CCreature class header.
 */

#ifndef __INCLUDE_CREATURE_H
#define __INCLUDE_CREATURE_H

#include <string>
#include "nullstream.h"
#include "thing.h"
#include "lang.h"
#include "functor.h"
#include "trans.h"

/**
 * Functor for parser of CCreature class.
 */
template<class CClass>
class CCmdFunctor: public CFunctor
{
private:
    /// Pointer to member function.
    void (CClass::*fpt) (const std::string&, const std::string&);
    /// Pointer to object.
    CClass* pt2Object;

public:
    /// Constructor. Takes pointer to an object and pointer to a member and stores
    /// them in two private variables.
    CCmdFunctor (CClass* _pt2Object, void (CClass::*_fpt) (const std::string&, const std::string&))
    {
        pt2Object = _pt2Object;  
        fpt = _fpt; 
    };

    /// Override operator "()".
    virtual void operator() (const std::string& string1, const std::string& string2)
    {
        (*pt2Object.*fpt) (string1, string2);           // execute member function
    };

    /// Override function "call".
    virtual void call (const std::string& string1, const std::string& string2)
    {
        (*pt2Object.*fpt) (string1, string2);           // execute member function
    };
};

typedef std::map<std::string, CFunctor*, CRuntimeStringCmp> CFunctorMap;
typedef CFunctorMap::iterator CFunctorMapIt;

typedef std::map<std::string, std::string, CRuntimeStringCmp> CStringMap;
typedef CStringMap::iterator CStringMapIt;

/**
 * Class for creatures, persons, players.
 */
class CCreature: public CThing
{
private:
    /// Is it spectator or not? (The Player is a spectator.)
    bool spectator;
    /// Command string and function map for parser.
    CFunctorMap parser_map;
    /// String map for aliases.
    CStringMap alias_map;
    /// Communication stream. Useful only if this creature is a spectator.
    std::ostream *ostream;
    /// The default output stream
    static NullStream nullstream;
    
    void init ();

public:
    // commands
    static const std::string CMD_SAY;
    static const std::string CMD_LOOK;
    static const std::string CMD_INVENTORY;
    static const std::string CMD_NORTH;
    static const std::string CMD_SOUTH;
    static const std::string CMD_EAST;
    static const std::string CMD_WEST;
    static const std::string CMD_NORTHEAST;
    static const std::string CMD_SOUTHEAST;
    static const std::string CMD_NORTHWEST;
    static const std::string CMD_SOUTHWEST;
    static const std::string CMD_IN;
    static const std::string CMD_OUT;
    static const std::string CMD_UP;
    static const std::string CMD_DOWN;
    static const std::string CMD_PICKUP;
    static const std::string CMD_DROP;
    static const std::string CMD_ALIAS;
    static const std::string CMD_BRINGOUT;
    static const std::string CMD_PUTAWAY;
    static const std::string CMD_POINTS;
    static const std::string CMD_ABOUT;
    static const std::string CMD_HELP;
    static const std::string CMD_HELP2;
    static const std::string CMD_INFO;
    // parameters
    static const std::string CMD_SELF;
    static const std::string CMD_ALL;
    static const std::string CMD_BRIEF;
    static const std::string CMD_VERBOSE;

    // CCreature keywords
    static const std::string K_SEX;
    static const std::string K_RANDOMMOVE;
    static const std::string K_WEAR;
    static const std::string K_RACE;
    static const std::string K_RANDOMSAY;
    // wearing types
    static const std::string K_LEFT_HAND;
    static const std::string K_RIGHT_HAND;
    static const std::string K_TWO_HANDS;

    static CThingList global_creaturelist;

    /// Constructor.
    CCreature ();
    /// Constructor.
    CCreature (const std::string& id, const std::string& name = "", 
            const std::string& descr = "", CThing *parent = NULL);
    /// Copy constructor.
    CCreature (CThing& thing);
    /// Copy constructor.
    CCreature (CCreature& thing);
    /// Destructor.
    virtual ~CCreature ();

    /**
     * After loading string, integer, float parameters must be call setup ().
     * This function find sChilds and copy them.
     */
    virtual void setup ();

    /**
     *  You must set output stream before enabling spectator mode.
     */
    void set_ostream (std::ostream *ostream);
    
    /**
     * Enable or disable spectator mode.
     * \param mode Spectator mode.
     */
    void set_spectator (bool mode);

    /**
     * Get spectator mode.
     * \return True if spectator mode enabled.
     */
    bool get_spectator ();

    /**
     * Initialize aliases.
     * \param aliases Format: "alias:command/alias2:command2/alias3:command3"
     */
    void set_aliases (std::string aliases);
    
    /**
     * Fetch aliases.
     * \return Format: "alias:command/alias2:command2/alias3:command3"
     */
    std::string get_aliases ();

    /**
     * Draw the shape of creature.
     * \return The shape of creature (usually one char).
     */
    virtual std::string get_shape ();
    
    /**
     * Get the color of creature.
     * \return The color of creature.
     */
    virtual int get_color ();

    /**
     * Non-player creature look somewho/something/around. Random event. Called from do_something.
     */
    virtual void random_look ();
    
    /**
     * Non-player creature says something. Random event. Called from do_something.
     */
    virtual void random_say ();

    /**
     * Non-player creature moves. Random event. Called from do_something.
     */
    virtual void random_move ();
    
    /**
     * Calling periodical by parent.
     */
    virtual void do_something ();

    /**
     * Command parser.
     * \param text Command in pure text format.
     * \param from_serial_number Origin of command. -1 or own serial number means: command gived by the player.
     */
    virtual bool parser (const std::string& text, signed long from_serial_number = -1);

    /**
     * Put text to spectators stream.
     * \param text Text to send.
     * \param except_thinglist Don't send text to the creatures in the list.
     * \param only_this_place If creature is in this place (pointer of parents equal) then send text.
     */
    void write_to_spectators (const std::string& text, CThingList& except_thinglist, bool only_this_place = true);

    /**
     * The creature says something.
     */
    void cmd_say (const std::string& cmd, const std::string& params);

    /**
     * The creature looks at something.
     */
    void cmd_look (const std::string& cmd, const std::string& params);

    /**
     * The creature draws up an inventory.
     */
    void cmd_inventory (const std::string& cmd, const std::string& params);

    /**
     * The creature moves.
     */
    void cmd_move (const std::string& cmd, const std::string& params);

    /**
     * The creature picks up something.
     */
    void cmd_pickup (const std::string& cmd, const std::string& params);

    /**
     * The creature drops something.
     */
    void cmd_drop (const std::string& cmd, const std::string& params);

    /**
     * Handling command aliases.
     */
    void cmd_alias (const std::string& cmd, const std::string& params);

    /**
     * Wear armours and clothes. Put on jewellery. Wield items. So bring out items.
     */
    void cmd_bringout (const std::string& cmd, const std::string& params);
    
    /**
     * Take off armours and clothes. Remove jewellery. So put away items.
     */
    void cmd_putaway (const std::string& cmd, const std::string& params);

    /**
     * Show statistics: strength, hit points, etc.
     */
    void cmd_points (const std::string& cmd, const std::string& params);

    /**
     * Show informations about author and program.
     */
    void cmd_about (const std::string& cmd, const std::string& params);

    /**
     * Shows help.
     */
    void cmd_help (const std::string& cmd, const std::string& params);
    
    /**
     * Shows debug info.
     */
    void cmd_info (const std::string& cmd, const std::string& params);
    
    /**
     * Calculate weight of creature (without load)
     */
    int get_weight ();

    /**
     * Calculate weight of load.
     */
    float get_load_weight ();

    /**
     * Calculate loadability of creature.
     */
    float get_loadability ();

    /**
     * Calculate specified statistic of creature.
     */
    virtual int get_stat (int stat_type);

    /**
     * Returns true if the creature is male.
     *
     * @return True: if it is a male.
     */
    bool isMale () { return get_iparam (K_SEX) == 0; };
    
    /**
     * Returns true if the creature is female.
     *
     * @return True: if it is a female.
     */
    bool isFemale () { return get_iparam (K_SEX) == 1; };

    /**
     * Returns true if the creature allowed to move around.
     *
     * @return True: if it can move.
     */
    bool doesRandomMove () { return get_iparam (K_RANDOMMOVE) == 1; };

    /**
     * Copy self.
     * \return Return the copy of this creature.
     */
    virtual CThing* copy () { return new CCreature (*this); };
}; // CCreature

#endif // __INCLUDE_CREATURE_H
