/*
 * File:        functor.h
 * Purpose:     CFunctor class header
 * Author:      Peter Ivanov
 * Modified by:
 * Created:     2005/04/18
 * Last modify: 2008-08-21 10:41:37 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2005
 * Licence:     GPL
 */
/**
 * \file functor.h
 * \brief CFunctor class header.
 */

#ifndef __INCLUDE_FUNCTOR_H
#define __INCLUDE_FUNCTOR_H

/** 
 * Abstract function object base (aka 'functor') class.
 * Example:
 * <pre>
// derived template class
template &lt;class CClass> 
class CSpecificFunctor : public CFunctor
{
private:
    void (CClass::*fpt) (const char*);          ///< Pointer to member function.
    CClass* pt2Object;                          ///< Pointer to object.

public:
    /// Constructor. Takes pointer to an object and pointer to a member and stores
    /// them in two private variables.
    CSpecificFunctor(CClass* _pt2Object, void(CClass::*_fpt)(const char*))
        { pt2Object = _pt2Object;  fpt=_fpt; };

    /// Override operator "()".
    virtual void operator() (const char* string)
        { (*pt2Object.*fpt)(string);};          // execute member function

    /// Override function "Call".
    virtual void call (const char* string)
        { (*pt2Object.*fpt)(string);};          // execute member function
};

// dummy class B
class CClassB{
public:

   CClassB(){};
   void Display(const char* text) { cout << text << endl; };

   // more of CClassB
};


// main program
int main(int argc, char* argv[])
{
   // 1. instantiate objects of CClassA and CClassB
   CClassA objA;
   CClassB objB;


   // 2. instantiate CSpecificFunctor objects ...
   //    a ) functor which encapsulates pointer to object and to member of CClassA
   CSpecificFunctor<CClassA> specFuncA(&objA, &CClassA::Display);

   //    b) functor which encapsulates pointer to object and to member of CClassB
   CSpecificFunctor<CClassB> specFuncB(&objB, &CClassB::Display);


   // 3. make array with pointers to CFunctor, the base class, and initialize it
   CFunctor* vTable[] = { &specFuncA, &specFuncB };


   // 4. use array to call member functions without the need of an object
   vTable[0]->Call("CClassA::Display called!");        // via function "Call"
   (*vTable[1])   ("CClassB::Display called!");        // via operator "()"


   // hit enter to terminate
   cout << endl << "Hit Enter to terminate!" << endl;
   cin.get();

   return 0;
}
 * </pre>
 *
 * Two possible functions to call member function. virtual cause derived
 * classes will use a pointer to an object and a pointer to a member function
 * to make the function call.
 */
class CFunctor
{
public:
    /// Call using operator.
    virtual void operator() (const std::string& string1, const std::string& string2)=0;
    /// Call using function.
    virtual void call (const std::string& string1, const std::string& string2)=0;
};

#endif // __INCLUDE_FUNCTOR_H
