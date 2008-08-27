/*
 * File:        loader.h
 * Purpose:     CLoader class header and inline implementation
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/13
 * Last modify: 2008-08-27 13:26:06 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file cfg.h
 * \brief CLoader class header.
 */

#ifndef __INCLUDE_LOADER_H
#define __INCLUDE_LOADER_H

#include "cfg.h"
#include "thing.h"
#include "split.h"
#include "trans.h"
#include "lang.h"
#include "keyword.h"

/**
 * Class for loading/unloading items, creatures and maps.
 */
template<class CAnyThing> 
class CLoader
{
public:
    CLoader (CThingList& thinglist, const std::string& filename) 
    { 
        load (thinglist, filename); 
    };
    CLoader () {};

    /**
     * Load the configuration file to thinglist.
     * \param thinglist Destination thinglist.
     * \param filename Name of the configuration file.
     * \return True: if load successful completed.
     */
    bool load (CThingList& thinglist, const std::string& filename);
    /**
     * Free the things from thinglist.
     */
    void unload (CThingList& thinglist);
};

template<class CAnyThing> 
inline
bool CLoader<CAnyThing>::load (CThingList& thinglist, const std::string& filename)
{
    CConfig cfg;
    CThing *thing = NULL;

    std::ostringstream os;
#if (LANG == ENG)
    os << "Loading " << filename << "...";
#endif
#if (LANG == HUN)
    os << filename << " betöltése...";
#endif
    Log.info (os.str ());
    
    if (!cfg.open (filename))
    {
        // cfg.open wrote error message
        //std::ostringstream os;
        //os << __INFO__ << "Can't open configuration file " << fn_cfg << " for reading!";
        //Log.error (os.str ());
        return false;
    }
    
    int i = 0;          // section counter
    while (cfg.ok ())
    {
        if (cfg.process ())
        {
            i++;
            std::string id = cfg.get_section ();
            // Process sChilds's data and others...
            if (thing)
            {
                thing->setup ();
            }
            thing = new CAnyThing;
            if (thing->find (id, thing->global_thinglist) != NULL)
            {
                os.str ("");
#if (LANG == ENG)
                os << __INFO__ << "'" << id << "' identifier is already defined! File: " << filename << 
                    " line: " << cfg.get_line_counter () << ".";
#endif
#if (LANG == HUN)
                os << __INFO__ << "'" << id << "' azonosító már használt! Fájl: " << filename << 
                    " sor: " << cfg.get_line_counter () << ".";
#endif
                Log.warn (os.str ());
            }
            thing->set_id (id);
            thinglist.push_back (thing);
        }
        std::string var, val;
        var = cfg.get_variable ();
        val = cfg.get_value ();
        if (var[0] == 's')
        {
            if (thing->sparams.count (var) > 0)
            {
                os.str ("");
#if (LANG == ENG)
                os << __INFO__ << "'" << var << "' variable is already defined! File: " << filename << 
                    " line: " << cfg.get_line_counter () << ".";
#endif
#if (LANG == HUN)
                os << __INFO__ << "'" << var << "' változó már kapott értéket! Fájl: " << filename << 
                    " sor: " << cfg.get_line_counter () << ".";
#endif
                Log.warn (os.str ());
            }
            thing->set_sparam (var, val);
        }
        if (var[0] == 'i')
        {
            if (thing->iparams.count (var) > 0)
            {
                os.str ("");
#if (LANG == ENG)
                os << __INFO__ << "'" << var << "' variable is already defined! File: " << filename << 
                    " line: " << cfg.get_line_counter () << ".";
#endif
#if (LANG == HUN)
                os << __INFO__ << "'" << var << "' változó már kapott értéket! Fájl: " << filename << 
                    " sor: " << cfg.get_line_counter () << ".";
#endif
                Log.warn (os.str ());
            }
            thing->set_iparam (var, atoi (val.c_str ()));
        }
        if (var[0] == 'f')
        {
            if (thing->fparams.count (var) > 0)
            {
                os.str ("");
#if (LANG == ENG)
                os << __INFO__ << "'" << var << "' variable is already defined! File: " << filename << 
                    " line: " << cfg.get_line_counter () << ".";
#endif
#if (LANG == HUN)
                os << __INFO__ << "'" << var << "' változó már kapott értéket! Fájl: " << filename << 
                    " sor: " << cfg.get_line_counter () << ".";
#endif
                Log.warn (os.str ());
            }
#if (LANG == HUN) && defined (GTKMM)
            // A magyarban tizedesvesszo van. A pontot automatikusan vesszore konvertaljuk.
            // Ha GTKMM definialva van az atof () a locale szerint mukodik.
            CTrans tr (".", ",");
#warning FIXME: meg mindig gond van a nyelvi beallitasokkal es az atof() fuggvennyel
            //tr (val);
#endif
            thing->set_fparam (var, atof (val.c_str ()));
        }
    }
    cfg.close ();
    
    // We don't forget to load the last thing's childs...
    if (thing)
    {
        thing->setup ();
    }
    
    os.str ("");
#if (LANG == ENG)
    os << "  " << i << " sections loaded.";
#endif
#if (LANG == HUN)
    os << "  " << i << " rész betöltve.";
#endif
    Log.info (os.str ());
    return true;
}

template<class CAnyThing> 
inline
void CLoader<CAnyThing>::unload (CThingList& thinglist)
{
    for (CThingListIt i = thinglist.begin (); i != thinglist.end (); i++)
    {
        // Deleting thing, and childs of thing
        delete (*i);
    }
}

#endif // __INCLUDE_LOADER_H
