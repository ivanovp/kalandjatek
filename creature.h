/*
 * File:        creature.h
 * Purpose:     CCreature class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-27 15:10:49 ivanovp {Time-stamp}
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
    static const std::string CREATURE; // for get_type()
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
    static const std::string CMD_USE;
    static const std::string CMD_PUTAWAY;
    static const std::string CMD_POINTS;
    static const std::string CMD_ATTACK;
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

    static const std::string K_ATTACKED;
    static const std::string K_DEAD;
    static const std::string K_HP_REGEN_COUNT;
    static const std::string K_MP_REGEN_COUNT;

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
     * \return true if spectator mode enabled.
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
     * If the creature is attacking somebody it will be handled here. Periodic event. Called from do_something.
     */
    virtual void combat ();

    /**
     * Increasing HP/MP. Periodic event. Called from do_something.
     */
    virtual void regenerate ();

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
     * \param except_thing Don't send text to the creature.
     * \param only_this_place If spectator creatures are in this place (pointer of parents are equal), we'll send text to them.
     */
    void write_to_spectators (const std::string& text, CThing& except_thing, bool only_this_place = true);

    /**
     * Put text to spectators stream.
     * \param text Text to send.
     * \param except_thinglist Don't send text to the creatures in the list.
     * \param only_this_place If spectator creatures are in this place (pointer of parents are equal), we'll send text to them.
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
    void cmd_use (const std::string& cmd, const std::string& params);
    
    /**
     * Take off armours and clothes. Remove jewellery. So put away items.
     */
    void cmd_putaway (const std::string& cmd, const std::string& params);

    /**
     * Show statistics: strength, hit points, etc.
     */
    void cmd_points (const std::string& cmd, const std::string& params);

    /**
     * Attack a creature.
     */
    void cmd_attack (const std::string& cmd, const std::string& params);

    /**
     * Show information about the author and the software.
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
     * Calculate damage of creature.
     */
    std::string get_damage ();

    /**
     * Calculate damage resistance of creature.
     */
    std::string get_damage_resistance ();

    /**
     * Calculate specified statistic of creature.
     */
    virtual int get_stat (int stat_type, bool base = true);

    /**
     * Returns true if the creature is male.
     *
     * @return true: if it is a male.
     */
    bool isMale () { return get_iparam (K_SEX) == 0; };
    
    /**
     * Returns true if the creature is female.
     *
     * @return true: if it is a female.
     */
    bool isFemale () { return get_iparam (K_SEX) == 1; };

    /**
     * Returns true if the creature is dead.
     *
     * @return true: if it is dead.
     */
    bool isDead () { return get_iparam (K_DEAD) == 1; };

    /**
     * Returns true if the creature is alive.
     *
     * @return true: if it is alive.
     */
    bool isAlive () { return get_iparam (K_DEAD) == 0; };

    /**
     * Returns true if the creature is in combat.
     *
     * @return true: if it is alive.
     */
    bool isInCombat () { return get_iparam (K_ATTACKED) != 0; };

    /**
     * Returns true if the creature allowed to move around.
     *
     * @return true: if it can move.
     */
    bool doesRandomMove () { return get_iparam (K_RANDOMMOVE) == 1; };

    /**
     * Copy self.
     * \return Return the copy of this creature.
     */
    virtual CThing* copy () { return new CCreature (*this); };
}; // CCreature

#endif // __INCLUDE_CREATURE_H
