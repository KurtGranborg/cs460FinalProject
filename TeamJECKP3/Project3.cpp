/*******************************************************************************
* Assignment: Project 3 - Syntactic Analyzer for Scheme to C++ Translator      *
* Author: Dr. Watts                                                            *
* Date: Fall 2017                                                              *
* File: Project3.cpp                                                           *
*                                                                              *
* Description: This file sets up SyntacticalAnalyzer/Sets Limist               *
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: %s <filename>\n", argv[0]);
		exit (1);
	}
    SetLimits ();
	SyntacticalAnalyzer parse (argv[1]);

	return 0;
}
