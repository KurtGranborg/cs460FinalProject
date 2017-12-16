/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                             *
* Date: Fall 2017                                                              *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the                                          *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "CodeGenerator.h"
#include <string>
using namespace std;

class SyntacticalAnalyzer
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	CodeGen * gen;
	ofstream p2file;
	token_type token;
	int Program ();
	int Define();
	int Action();
	int Any_Other_Token();
	int Else_Part();
	int Literal();
	int More_Defines();
	int More_Pairs();
	int More_Tokens();
	int Param_List();
	int Quoted_Lit();
	int Stmt();
	int Stmt_List(string, bool);
	int Stmt_Pair(string);
	int Stmt_Pair_Body();
	int tabs;
  bool printedReturn;
};

#endif
