
/*******************************************************************************
* Assignment: Project 1 - Syntactic Analyzer for Scheme to C++ Translator      *
* Authors: Kurt Granborg, Eli Simmonds, Charlie Moore                          *
* Based on Project 2 code by: Team V-Brooke Gardner, Kurt Granborg, John Eggers*
* Date: Fall 2017                                                              *
* File: Syntactical Analyzer.cpp                                               *
*                                                                              *
* Description: This file contains the function implementations for             *
* SyntacticAnalyzer.h                                                          *
*                                                                              *
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "SyntacticalAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

/*******************************************************************************
* Function: Defualt constructor 					       *
*                                                                              *
* Parameters: filename for input and output                                    *
* Return value: None                                                           *
* Description: This function will create LexicalAnalyzer,                      *
* and open output p2file                                                       *
*******************************************************************************/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
       	string name = filename;
	string p2name = name.substr (0, name.length()-3) + ".p2";
	gen = new CodeGen ( name, lex);
	p2file.open (p2name.c_str());
	token = lex->GetToken();
	Program ();
}

/*******************************************************************************
* Function: Destructor                                                         *
*                                                                              *
* Parameters: None                                                             *
* Return value: none                                                           *
* Description: This function will close file, delete LexicalAnalyzer           *
*******************************************************************************/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	delete gen;
	p2file.close ();
}

/*******************************************************************************
* Function: Program                                                            *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <program>                                                   *
*******************************************************************************/
int SyntacticalAnalyzer::Program ()
{

	p2file << "Entering Program function; current token is: "
	       << lex->GetTokenName (token)
	       << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;

	p2file << "Using Rule 1" << endl;

	//call function to examine grammar rules for nonterminals <define> and
	// <more_defines>
	errors += Define ();
	errors += More_Defines ();
	//should be EOF when it comes back from More_Defines()
	if (token != EOF_T)
	{
		errors++;
		lex->ReportError ("Missing end of file at end of program");
	}
	p2file << "Exiting Program function; current token is: "
					<< lex->GetTokenName (token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Define                                                             *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <define>                                                    *
*******************************************************************************/

int SyntacticalAnalyzer::Define(){

	p2file << "Entering Define function; current token is: "
	<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	p2file << "Using Rule 2" << endl;
	printedReturn = false;
	int errors = 0;
	//follow define recipe
	if(token == LPAREN_T){
		token = lex->GetToken();
	}
	else{
		//found error
		//report error and forget about it
		errors++;
		lex->ReportError("Expecting \"(\"; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}
	if(token == DEFINE_T){
		token = lex->GetToken();
	}
	else{
		errors++;
		lex->ReportError("Expecting \"define\"; saw" + lex->GetLexeme());
		token = lex->GetToken();
	}

	if(token == LPAREN_T){
		token = lex->GetToken();
	}
	else{
		errors++;
		lex->ReportError("Expecting \"(\"; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}

	if(token == IDENT_T){
		if (lex->GetLexeme() == "main")
		  gen->WriteCode(tabs++, "int main (");
		else
		  gen->WriteCode(0, "Object " + lex->GetLexeme() + " (");
		token = lex->GetToken();
	}
	else{
		errors++;
		lex->ReportError("Expecting identifiers; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}

	//call grammar rules for <param_list>
	errors += Param_List(false);
	gen->WriteCode(0, ")\n{\n");
	if(token == RPAREN_T){
		token = lex->GetToken();
	}
	else{
		errors++;
		lex->ReportError("Expecting \")\"; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}

	//call grammar rules for <stmt> and <stmt_list>
	errors += Stmt("");
	gen->WriteCode(0, "\n");
	errors += Stmt_List("\n", false);

	if(token == RPAREN_T){
		token = lex->GetToken();
		gen->WriteCode(0, "}\n");
	}
	else{
		errors++;
		lex->ReportError("Expecting \")\"; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}

	p2file << "Exiting Define function; current token is: "
	<< lex->GetTokenName(token) << endl;
	tabs--;
	return errors;
}

/*******************************************************************************
* Function: Action                                                             *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <action>                                                    *
*******************************************************************************/

int SyntacticalAnalyzer::Action(){

	p2file << "Entering Action function; current token is: "
		<< lex->GetTokenName (token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	//check firsts to determine which rules to use
	if(token == DISPLAY_T){
		p2file << "Using Rule 50" << endl;
		token = lex->GetToken();
		gen->WriteCode(0, "cout << ");
		errors += Stmt("");
		gen->WriteCode(0, ";");

	}
	else if(token == NEWLINE_T){
		p2file << "Using Rule 51" << endl;
		gen->WriteCode(0, "cout << endl");
		token = lex->GetToken();
		gen->WriteCode(0, ";");
	}
	else if(token == IF_T){
		p2file << "Using Rule 26" << endl;
		token = lex->GetToken();
		gen->WriteCode(0, "if(");
		errors += Stmt("");
		gen->WriteCode(0, ") {\n");
		errors += Stmt("");
		gen->WriteCode(0, "\n}");
		errors += Else_Part();
	}else if(token == COND_T){
		p2file << "Using Rule 27" << endl;
		token = lex->GetToken();
		errors += Stmt_Pair("");
		errors += More_Pairs();
		gen->WriteCode(0, " );\n");
	}else if(token == AND_T){
		p2file << "Using Rule 30" << endl;
		token = lex->GetToken();
		errors += Stmt_List(" && ", false);
	}
	else if(token == OR_T){
		p2file << "Using Rule 31" << endl;
		token = lex->GetToken();
		errors += Stmt_List(" || ", false);
	}
	else if(token == NOT_T){
		p2file << "Using Rule 32" << endl;
		token = lex->GetToken();
		gen->WriteCode(0, " !(");
		errors += Stmt("");
		gen->WriteCode(0, ") ");

	}
	else if(token == NUMBERP_T){
		p2file << "Using Rule 33" << endl;
		gen->WriteCode(0, "numberp( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");

	}
	else if(token == SYMBOLP_T){
		p2file << "Using Rule 34" << endl;
		gen->WriteCode(0, "symbolp( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");
	}
	else if(token == LISTP_T){
		p2file << "Using Rule 35" << endl;
		gen->WriteCode(0, "listp( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");
	}
	else if(token == ZEROP_T){
		p2file << "Using Rule 36" << endl;
		gen->WriteCode(0, "zerop( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");
	}
	else if(token == NULLP_T){
		p2file << "Using Rule 37" << endl;
		gen->WriteCode(0, "nullp( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");
	}
	else if(token == STRINGP_T){
		p2file << "Using Rule 38" << endl;
		gen->WriteCode(0, "stringp( ");
		token = lex->GetToken();
		errors += Stmt("");
		gen->WriteCode(0, " )");
	}else if(token == EQUALTO_T){
		p2file << "Using Rule 44" << endl;
		token = lex->GetToken();
		errors += Stmt_List( " == ", false);
	}
	else if(token == GT_T){
		p2file << "Using Rule 45" << endl;
		token = lex->GetToken();
		errors += Stmt_List(" > ", false);
	}
	else if(token == LT_T){
		p2file << "Using Rule 46" << endl;
		token = lex->GetToken();
		errors += Stmt_List( " < ", false);
	}
	else if(token == GTE_T){
		p2file << "Using Rule 47" << endl;
		token = lex->GetToken();
		errors += Stmt_List(" >= ", false);
	}
	else if(token == LTE_T){
		p2file << "Using Rule 48" << endl;
		token = lex->GetToken();
		errors += Stmt_List( " <= ", false);
	}
	else{
		gen->WriteCode(0, "Object(");
		if(token == LISTOP_T){
			p2file << "Using Rule 28" << endl;
			string symbol = lex->GetLexeme();
			token = lex->GetToken();
			gen->WriteCode(0, "listop(\"" + symbol + "\", ");
			errors += Stmt("");
			gen->WriteCode(0, ")");
		}
		else if(token == CONS_T){
			p2file << "Using Rule 29" << endl;
			token = lex->GetToken();
			gen->WriteCode(0, "cons(");
			errors += Stmt("");
			gen->WriteCode(0, ", ");
			errors += Stmt("");
			gen->WriteCode(0, ")");
		}
		else if(token == PLUS_T){
			p2file << "Using Rule 39" << endl;
			token = lex->GetToken();
			errors += Stmt_List(" + ", false);
		}
		else if(token == MINUS_T){
			p2file << "Using Rule 40" << endl;
			token = lex->GetToken();
			errors += Stmt("");
			errors += Stmt_List(" - ", true);
		}
		else if(token == DIV_T){
			p2file << "Using Rule 41" << endl;
			token = lex->GetToken();
			errors += Stmt("");
			errors += Stmt_List(" / ", true);
		}
		else if(token == MULT_T){
			p2file << "Using Rule 42" << endl;
			token = lex->GetToken();
			errors += Stmt_List(" * ", false);
		}
		else if(token == MODULO_T){
			p2file << "Using Rule 43" << endl;
			token = lex->GetToken();
			errors += Stmt("");
			gen->WriteCode(0, " % ");
			errors += Stmt("");
		}
		else if(token == IDENT_T){
			p2file << "Using Rule 49" << endl;\
			gen->WriteCode(0, lex->GetLexeme() + "(");
			string symbol = lex->GetLexeme();
			token = lex->GetToken();
			errors += Stmt_List("", false);
			gen->WriteCode(0, ")");
		}
		else{
			errors++;
			lex->ReportError("Expecting a first for action; saw " + lex->GetLexeme());
			token = lex->GetToken();
		}
		gen->WriteCode(0, ")");
	}
	p2file << "Exiting Action function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Any_Other_Token                                                    *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <any_other_token>                                           *
*******************************************************************************/

int SyntacticalAnalyzer::Any_Other_Token(){

	p2file << "Entering Any_Other_Token function; current token is: "
               << lex->GetTokenName(token)
               << ", lexeme: " << lex->GetLexeme() << endl;

	int errors = 0;

	if(token == LPAREN_T){
		p2file << "Using Rule 52" << endl;
		gen->WriteCode(0, "(");
		token = lex->GetToken();
		gen->WriteCode(0, ", ");
		errors += More_Tokens();

		if(token == RPAREN_T){
			gen->WriteCode(0, ")");
			token = lex->GetToken();
		}
		else{
			errors++;
			lex->ReportError("Expecting \")\"; saw " + lex->GetLexeme());
			token = lex->GetToken();
		}
	}
	else{
		gen->WriteCode(0, lex->GetLexeme());
		if(token == IDENT_T){
			p2file << "Using Rule 53" << endl;
			token = lex->GetToken();
		}
		else if(token == NUMLIT_T){
			p2file << "Using Rule 54" << endl;
			token = lex->GetToken();
		}
		else if(token == STRLIT_T){
			p2file << "Using Rule 55" << endl;
			token = lex->GetToken();
		}
		else if(token == CONS_T){
			p2file << "Using Rule 56" << endl;
			token = lex->GetToken();
		}
		else if(token == IF_T){
			p2file << "Using Rule 57" << endl;
			token = lex->GetToken();
		}
		else if(token == DISPLAY_T){
			p2file << "Using Rule 58" << endl;
			token = lex->GetToken();
		}
		else if(token == NEWLINE_T){
			p2file << "Using Rule 59" << endl;
			token = lex->GetToken();
		}
		else if(token == LISTOP_T){
			p2file << "Using Rule 60" << endl;
			token = lex->GetToken();
		}
		else if(token == AND_T){
			p2file << "Using Rule 61" << endl;
			token = lex->GetToken();
		}
		else if(token == OR_T){
			p2file << "Using Rule 62" << endl;
			token = lex->GetToken();
		}
		else if(token == NOT_T){
			p2file << "Using Rule 63" << endl;
			token = lex->GetToken();
		}
		else if(token == DEFINE_T){
			p2file << "Using Rule 64" << endl;
			token = lex->GetToken();
		}
		else if(token == NUMBERP_T){
			p2file << "Using Rule 65" << endl;
			token = lex->GetToken();
		}
		else if(token == SYMBOLP_T){
			p2file << "Using Rule 66" << endl;
			token = lex->GetToken();
		}
		else if(token == LISTP_T){
			p2file << "Using Rule 67" << endl;
			token = lex->GetToken();
		}
		else if(token == ZEROP_T){
			p2file << "Using Rule 68" << endl;
			token = lex->GetToken();
		}
		else if(token == NULLP_T){
			p2file << "Using Rule 69" << endl;
			token = lex->GetToken();
		}
		else if(token == STRINGP_T){
			p2file << "Using Rule 70" << endl;
			token = lex->GetToken();
		}
		else if(token == PLUS_T){
			p2file << "Using Rule 71" << endl;
			token = lex->GetToken();
		}
		else if(token == MINUS_T){
			p2file << "Using Rule 72" << endl;
			token = lex->GetToken();
		}
		else if(token == DIV_T){
			p2file << "Using Rule 73" << endl;
			token = lex->GetToken();
		}
		else if(token == MULT_T){
			p2file << "Using Rule 74" << endl;
			token = lex->GetToken();
		}
		else if(token == MODULO_T){
			p2file << "Using Rule 75" << endl;
			token = lex->GetToken();
		}
		else if(token == EQUALTO_T){
			p2file << "Using Rule 76" << endl;
			token = lex->GetToken();
		}
		else if(token == GT_T){
			p2file << "Using Rule 77" << endl;
			token = lex->GetToken();
		}
		else if(token == LT_T){
			p2file << "Using Rule 78" << endl;
			token = lex->GetToken();
		}
		else if(token == GTE_T){
			p2file << "Using Rule 79" << endl;
			token = lex->GetToken();
		}
		else if(token == LTE_T){
			p2file << "Using Rule 80" << endl;
			token = lex->GetToken();
		}
		else if(token == QUOTE_T){
			p2file << "Using Rule 81" << endl;
			token = lex->GetToken();
			errors += Any_Other_Token();
		}
		else{
			errors++;
			lex->ReportError("Expecting a first for any_other_token; saw "
				+ lex->GetLexeme());
			token = lex->GetToken();
		}
	}
	p2file << "Exiting Any_Other_Token function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Else_Part                                                          *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <else_part>                                                 *
*******************************************************************************/

int SyntacticalAnalyzer::Else_Part(){
	p2file << "Entering Else_Part function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;

    	if(token == IDENT_T
        	|| token == LPAREN_T
        	|| token == NUMLIT_T
        	|| token == STRLIT_T
        	|| token == QUOTE_T)
					{
        	p2file << "Using Rule 18" << endl;
					gen->WriteCode(0, "else{\n");
	        errors += Stmt("");
					gen->WriteCode(0, "\n}\n");
    	}
    	else if(token == RPAREN_T){
        	p2file << "Using Rule 19" << endl;
    	}
    	else{
        	errors++;
        	lex->ReportError("Unexpected token: " + lex->GetLexeme());
	   	token = lex->GetToken();
   	 }
	p2file << "Exiting Else_Part function; current token is: "
		<< lex->GetTokenName(token) << endl;
    	return errors;
}

/*******************************************************************************
* Function: Literal                                                            *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <Literal>                                                   *
*******************************************************************************/

int SyntacticalAnalyzer::Literal(){
	p2file << "Entering Literal function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;

	if(token == NUMLIT_T){
		p2file << "Using Rule 10" << endl;
		gen->WriteCode(0, " Object(" + lex->GetLexeme() + ") ");
		token = lex->GetToken();
	}
	else if (token == STRLIT_T){
		p2file << "Using Rule 11" << endl;
		gen->WriteCode(0, " Object(" + lex->GetLexeme() + ") ");
		token = lex->GetToken();
	}
	else if (token == QUOTE_T){
		p2file << "Using Rule 12" << endl;
		token = lex->GetToken();
		errors += Quoted_Lit();
	}
	else{
		errors++;
		lex->ReportError("Expeciting a first for Literal; saw" + lex->GetLexeme());
		token = lex->GetToken();
	}
	p2file << "Exiting Literal function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}


/*******************************************************************************
* Function: More_Defines                                                       *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <more_defines>                                              *
*******************************************************************************/
int SyntacticalAnalyzer::More_Defines(){
	p2file << "Entering More_Defines function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
 	//check to see if a first...
    	if(token == LPAREN_T){
        	p2file << "Using Rule 3" << endl;
		//notice that we don't get another token here
        	errors += Define();
		errors += More_Defines();
    	}
	//look through to see if we have a follow....
    	else if(token == EOF_T)
        	//-> {} means do nothing
		p2file << "Using Rule 4" << endl;
    	else{
        	lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
        	errors++;
    	}
	p2file << "Exiting More_Defines function; current token is: "
		<< lex->GetTokenName(token) << endl;
    	return errors;
}

/*******************************************************************************
* Function: More_Pairs                                                         *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <more_pairs>                                                *
*******************************************************************************/

int SyntacticalAnalyzer::More_Pairs(){

	p2file << "Entering More_Pairs function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
    	if(token == LPAREN_T) {
        	p2file << "Using Rule 24" << endl;
        	errors += Stmt_Pair("");
		errors += More_Pairs();
    	}else if(token == RPAREN_T)
		// {} means do nothing
        	p2file << "Using Rule 25" << endl;
    	else{
        	lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
        	errors++;
	   	token = lex->GetToken();
    }
    p2file << "Exiting More_Pairs function; current token is: "
			<< lex->GetTokenName(token) << endl;
    return errors;
}

/*******************************************************************************
* Function: More_Tokens                                                        *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <more_tokens>                                               *
*******************************************************************************/

int SyntacticalAnalyzer::More_Tokens(){
	p2file << "Entering More_Tokens function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
    if(token == LPAREN_T
            ||token == IDENT_T
            || token == NUMLIT_T
            || token == STRLIT_T
            || token == CONS_T
            || token == IF_T
            || token == DISPLAY_T
            || token == NEWLINE_T
            || token == LISTOP_T
            || token == AND_T
            || token == OR_T
            || token == NOT_T
            || token == DEFINE_T
            || token == NUMBERP_T
            || token == SYMBOLP_T
            || token == LISTP_T
            || token == ZEROP_T
            || token == NULLP_T
            || token == STRINGP_T
            || token == PLUS_T
            || token == MINUS_T
            || token == DIV_T
            || token == MULT_T
            || token == MODULO_T
            || token == EQUALTO_T
            || token == GT_T
            || token == LT_T
            || token == GTE_T
            || token == LTE_T
            || token == QUOTE_T){
        p2file << "Using Rule 14" << endl;
        errors += Any_Other_Token();
	errors += More_Tokens();
    }else if(token == RPAREN_T)
        p2file << "Using Rule 15" << endl;
    else{
        errors++;
        lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
        token = lex->GetToken();
    }
    p2file << "Exiting More_Tokens function; current token is: "
			<< lex->GetTokenName(token) << endl;
    return errors;
}

/*******************************************************************************
* Function: Param_List                                                         *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <param_list>                                                *
*******************************************************************************/

int SyntacticalAnalyzer::Param_List(bool needComma){
	p2file << "Entering Param_List function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
    if(token == IDENT_T){
        p2file << "Using Rule 16" << endl;
	if(needComma)
	  gen->WriteCode(0, ", ");
	gen->WriteCode(0, "Object " + lex->GetLexeme());
        token = lex->GetToken();
        errors = Param_List(true);
    }else if(token == RPAREN_T){
        p2file << "Using Rule 17" << endl;
	needComma = false;
    }else{
        lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
        errors++;
	   token = lex->GetToken();
    }
	p2file << "Exiting Param_List function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Quoted_Lit                                                         *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <quoted_lit>                                                *
*******************************************************************************/

int SyntacticalAnalyzer::Quoted_Lit(){
	p2file << "Entering Quoted_Lit function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	p2file << "Using Rule 13" << endl;
	gen->WriteCode(0, " Object(\"");
	errors += Any_Other_Token();
	gen->WriteCode(0, "\") ");
	p2file << "Exiting Quoted_Lit function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Stmt                                                               *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <stmt>                                                      *
*******************************************************************************/

int SyntacticalAnalyzer::Stmt(string symbol){
	p2file << "Entering Stmt function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if(token == IDENT_T){
		p2file << "Using Rule 8" << endl;
		gen->WriteCode(0, lex->GetLexeme());
		token = lex->GetToken();
	}
	else if(token == LPAREN_T){
		p2file << "Using Rule 9" << endl;
		token = lex->GetToken();
		errors += Action();
		if(token == RPAREN_T){
			token = lex->GetToken();
		}
		else{
			errors++;
			lex->ReportError("Expecting ); saw " + lex->GetLexeme());
			token = lex->GetToken();
		}
	}
	else{
		p2file << "Using Rule 7" << endl;
		errors += Literal();
	}
	//check follows?
	p2file << "Exiting Stmt function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}

/*******************************************************************************
* Function: Stmt_List                                                          *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <stmt_list>                                                 *
*******************************************************************************/

int SyntacticalAnalyzer::Stmt_List(string symbol, bool printSymbol){
	p2file << "Entering Stmt_List function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if(token == IDENT_T
	  || token == LPAREN_T
	   || token == NUMLIT_T
	   || token == STRLIT_T
	   || token == QUOTE_T){
	    p2file << "Using Rule 5" << endl;
			if (printSymbol)
				gen->WriteCode(0, " " + symbol + " ");
	    errors += Stmt("");
	    errors += Stmt_List(symbol, true);
	}
	else if (token == RPAREN_T){
		p2file << "Using Rule 6" << endl;
	}
	else{
	lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
        errors++;
	   token = lex->GetToken();
	}
	p2file << "Exiting Stmt_List function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;

}

/*******************************************************************************
* Function: Stmt_Pair                                                          *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <stmt_pair>                                                 *
*******************************************************************************/

int SyntacticalAnalyzer::Stmt_Pair(string symbol){
	p2file << "Entering Stmt_Pair function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if(token == LPAREN_T){
		p2file << "Using Rule 20" << endl;
		token = lex->GetToken();
		errors += Stmt_Pair_Body();
		if(token == RPAREN_T){
			token = lex->GetToken();
		}
		else{
			errors++;
			lex->ReportError("Expecting ); saw " + lex->GetLexeme());
			token = lex->GetToken();
		}
	}
	else{
		errors++;
		lex->ReportError("Expecting (; saw " + lex->GetLexeme());
		token = lex->GetToken();
	}
	p2file << "Exiting Stmt_Pair function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;

}

/*******************************************************************************
* Function: Stmt_Pair_Body()                                                   *
*                                                                              *
* Parameters: none                                                             *
* Return value: count of errors found                                          *
* Description: This function will examine the order of tokens found by the     *
* LexicalAnalyzer and determine if it is appropriate for grammar rules starting*
* with nonterminal <stmt_pair_body>                                            *
*******************************************************************************/

int SyntacticalAnalyzer::Stmt_Pair_Body(){
	p2file << "Entering Stmt_Pair_Body function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	if(token == LPAREN_T){
		p2file << "Using Rule 21" << endl;
		token = lex->GetToken();
		gen->WriteCode(0, "if(");
		errors += Action();
		gen->WriteCode(0, "){\n");
		if(token == RPAREN_T){
			token = lex->GetToken();
		}
		else{
			lex->ReportError("Expecting ); saw " + lex->GetLexeme());
			errors += 1;
		}
		errors += Stmt("");
		gen->WriteCode(0, "}\n");
	}
	else if(token == NUMLIT_T
        || token == STRLIT_T
        || token == QUOTE_T){
        p2file << "Using Rule 23" << endl;
        errors += Literal();
	}else  if(token == IDENT_T
              || token == CONS_T
              || token == IF_T
              || token == DISPLAY_T
              || token == NEWLINE_T
              || token == LISTOP_T
              || token == AND_T
              || token == OR_T
              || token == NOT_T
              || token == DEFINE_T
              || token == NUMBERP_T
              || token == SYMBOLP_T
              || token == LISTP_T
              || token == ZEROP_T
              || token == NULLP_T
              || token == STRINGP_T
              || token == PLUS_T
              || token == MINUS_T
              || token == DIV_T
              || token == MULT_T
              || token == MODULO_T
              || token == EQUALTO_T
              || token == GT_T
              || token == LT_T
              || token == GTE_T
              || token == LTE_T) {
        p2file << "Using Rule 22" << endl;
        errors += Action();
    }
	else{
		lex->ReportError("Unexpected Token; saw " + lex->GetLexeme());
    errors++;
	  token = lex->GetToken();
	}
	p2file << "Exiting Stmt_Pair_Body function; current token is: "
		<< lex->GetTokenName(token) << endl;
	return errors;
}
