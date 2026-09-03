
/******************************************************************************/
/* Main.Cpp: C++ ostream + stl + exception demonstration                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2007 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "Debug_Uart.h"
#include <string.h>
//#include <stdio.h>
#include <rt_misc.h>

//#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stddef.h>

extern "C" {
	void stl_test (void) ;
	void Dprintf(char *fmt, ...);     
}

typedef unsigned int  U32;

/*
 * STL Test Code
 */

typedef std::vector<int>       IntVectorType;
typedef IntVectorType::iterator IntVectorItr;
typedef std::map<int, int>     IntIntMapType;
typedef IntIntMapType::iterator IntIntMapItr;

IntVectorType v;
IntIntMapType m;
//std::string str;

void stl_test (void)  
{
	//str = "Test";
  int   sum = 0;
    Dprintf("STL Test.....\n");
// vector test
  sum = v.size();
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  sum = v.size();
	
  sum = 0;
  for (IntVectorItr itr = v.begin(); itr != v.end(); itr++)  {
    sum += *itr;
  }
  Dprintf ("*** stl_test: vector sum = %d\n", sum);

// map test
  sum  = 0;
  m[1] = 1;
  m[2] = 2;
  m[3] = 3;
	
  for (IntIntMapItr itr = m.begin(); itr != m.end(); itr++)  {
    sum += itr->first * itr->second;
  }
  Dprintf ("*** stl_test: map sum = %d\n", sum);

// Exception-test:
//  Note: execptions need to be enabled in
//   Options for Target - C/C++ - Misc controls: '--exceptions'				 
  //std::cout << "Throwing an exception:" << std::endl;
  Dprintf ("----------------------\n");


  /*
  if (str.length() > 0)  {
    str += " - has been catched !";
  }
  
  Dprintf("%s\n", str.c_str());
  */
}

