/*
 * File:        keyword.h
 * Purpose:     Keyword definitions
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/17
 * Last modify: 2008-08-22 09:46:26 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file keyword.h
 * \brief Definitions of configuration keywords.
 */

#ifndef __INCLUDE_KEYWORD_H
#define __INCLUDE_KEYWORD_H

// Common keywords
#define K_LISTSEPARATOR "/"             // sStringList = "first/second/third"
#define K_LISTSEPARATOR2 ":"            // sStringList = "valami:first/foo:second/thing:third"

// CApp keywords (testcfg.cfg)
#define K_WINDOW_X      "iWindowX"
#define K_WINDOW_Y      "iWindowY"
#define K_WINDOW_W      "iWindowW"
#define K_WINDOW_H      "iWindowH"
#define K_AUTO_SCROLL   "iAutoScroll"
#define K_ITEM_CFG      "sItemCfg"
#define K_CREATURE_CFG  "sCreatureCfg"
#define K_MAP_CFG       "sMapCfg"
#define K_PLAYER_ID     "sPlayerID"
#define K_FONT          "sFont"
#define K_ALIASES       "sAliases"      // parancs álnevek

// common CThing keywords
//#define K_NAME          "sName"
//#define K_DESCR         "sDescr"
//#define K_ALTNAME       "sAltName"
//#define K_RAGOK         "sRagok"        // hungarian endings
//#define K_PLURAL        "sPlural"       // többes szám
//#define K_CHILDS        "sChilds"       // possessions
//#define K_BODY          "iBody"
//#define K_STRENGTH      "iStrength"
//#define K_DEXTERITY     "iDexterity"
//#define K_PERCEPTION    "iPerception"
//#define K_INTELLIGENCE  "iIntelligence"
//#define K_WILLPOWER     "iWillpower"
//#define K_CHARISMA      "iCharisma"
//#define K_BDY           K_BODY
//#define K_STR           K_STRENGTH
//#define K_DEX           K_DEXTERITY
//#define K_PER           K_PERCEPTION
//#define K_INT           K_INTELLIGENCE
//#define K_WIL           K_WILLPOWER
//#define K_CHA           K_CHARISMA
//#define K_HP            "iHP"
//#define K_MP            "iMP"
////#define K_CURR_HP       "iCurrentHP"
////#define K_CURR_MP       "iCurrentMP"
//#define K_ATTACK        "iAttack"
//#define K_DEFENSE       "iDefense"
// CItem keywords
//#define K_PRICE         "fPrice"        // ár
//#define K_WEIGHT        "fWeight"       // tömeg
//#define K_MOVABLE       "iMovable"      // mozgatható/felvehetõ tárgy
//#define K_VISIBLE       "iVisible"      // ,,néz'' paranccsal látható
//#define K_WEAREDON      "sWearedOn"     // min viseli a tárgyat (nem a konfig fájlban használatos)
// CMap keywords
#define K_EXITS         "sExits"        // kijáratok
// CCreature keywords
#define K_WEAR          "sWear"         // viselt dolgok
#define K_RACE          "sRace"         // faj (inkább másképp kellene megoldani)
#define K_NOUN          "iNoun"         // köznév/tulajdonnév
#define K_RANDOMSAY     "sRandomSay"    // véletlenszerûen mondott szövegek
//#define K_RANDOMMOVE    "iRandomMove"   // mozog-e a fickó
// wearing types
#define K_LEFT_HAND     "left hand"     // bal kéz
#define K_RIGHT_HAND    "right hand"    // jobb kéz
#define K_TWO_HANDS     "two hands"     // két kéz
// és az alábbiak is

// CItem keywords
#define K_TYPE          "sType"

#define K_RING          "ring"          // gyűrű
#define K_BRACELET      "bracelet"      // karkötõ
#define K_AMULET        "amulet"        // nyaklánc
#define K_CLOAK         "cloak"         // köpeny/csuha
#define K_PANTS         "pants"         // nadrág
#define K_FOOTWEAR      "footwear"      // lábbeli
#define K_GLOVE         "glove"         // kesztyû 
#define K_CAP           "cap"           // kalap/sapka
#define K_TORSO         "torso"         // törzs/torzó (mellvért)
#define K_ARMGUARDS     "armguards"     // alkarvédõ
#define K_SHINGUARDS    "shinguards"    // lábszárvédõ
#define K_ONE_HANDED    "one handed"    // egykezes (fegyver)
#define K_TWO_HANDED    "two handed"    // kétkezes (fegyver)
#define K_CURRENCY      "currency"      // fizetõeszköz
#define K_KEY           "key"           // kulcs


#endif // __INCLUDE_KEYWORD_H
