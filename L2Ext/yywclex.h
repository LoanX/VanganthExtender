#ifndef YYWCLEX_H
#define YYWCLEX_H

#pragma warning(disable: 4103)

/************************************************************
yywclex.h
This file can be freely modified for the generation of
custom code.

Copyright (c) 1999-2003 Bumble-Bee Software Ltd.
************************************************************/

// globals
#include "yyglobal.h"

// standard include files
#ifdef YYSTDCPPLIB
#include <iostream>
#include <cstdio>
#else
#include <stdio.h>
#endif

// defines
#include "yytdefs.h"

// user defines
#if defined(YYTUDEFS) || defined(YYUDEFS)
#include "yytudefs.h"
#endif

#ifndef YYSTD
#ifdef YYSTDCPPLIB
#define YYSTD std::
#else
#define YYSTD
#endif
#endif

#ifndef YYCSTD
#ifdef YYCSTDCPPLIB
#define YYCSTD std::
#else
#define YYCSTD
#endif
#endif

// namespace
#ifdef YYNAMESPACE
#ifndef _YL_NAME
#define _YL_NAME	yl
#endif
#ifndef _YL
#define _YL			yl::
#endif
#ifndef _YL_BEGIN
#define _YL_BEGIN	namespace yl {
#endif
#ifndef _YL_END
#define _YL_END		}
#endif
#else
#ifndef _YL_NAME
#define _YL_NAME
#endif
#ifndef _YL
#define _YL
#endif
#ifndef _YL_BEGIN
#define _YL_BEGIN
#endif
#ifndef _YL_END
#define _YL_END
#endif
#endif

_YL_BEGIN

#define YY_ALEX

// modifiers
#ifndef YYEXPCLASS
#define YYEXPCLASS
#endif
#ifndef YYEXPDATA
#define YYEXPDATA(x) x
#endif
#ifndef YYEXPFUNC
#define YYEXPFUNC(x) x
#endif
#ifndef YYCDECL
#define YYCDECL
#endif
#ifndef YYDCDECL
#define YYDCDECL
#endif
#ifndef YYNEAR
#define YYNEAR
#endif
#ifndef YYBASED_CODE
#define YYBASED_CODE
#endif

// forward references
class YYEXPCLASS  yywparser;

// yylex return values
#define YYEOF 0			// end of file

#ifndef YYWSTATE_T
#define YYWSTATE_T
typedef struct yywstate {
	short yydef;		// default state
	long yybase;		// base
	short yymatch;		// action associated with state
} yywstate_t;
#endif

#ifndef YYWTRANSITION_T
#define YYWTRANSITION_T
typedef struct yywtransition {
	short yynext;		// next state on transition
	short yycheck;		// check
} yywtransition_t;
#endif

#ifndef YYWCTRANSITION_T
#define YYWCTRANSITION_T
typedef struct yywctransition {
	wchar_t yyfirst;		// first character in range
	wchar_t yylast;			// last character in range
	short yynext;			// next state on transition
} yywctransition_t;
#endif

#ifndef YYWINT_T
#define YYWINT_T
#ifdef YYWIN32
typedef int yywint_t;
#else
typedef long yywint_t;
#endif
#endif

typedef short yywmatch_t;
typedef unsigned char yywbackup_t;

class YYEXPCLASS  yywlexer {
public:
	yywlexer();
	virtual ~yywlexer();

// Attributes
protected:
	// left context
	int yystart;					// current start state
	yybool yyeol;					// whether an end-of-line '\n' has been seen
	yybool yyoldeol;				// previous end-of-line value

	// text buffer
	int * yystatebuf;			// state buffer
	int * yysstatebuf;			// initial (static) state buffer
	wchar_t * yystext;			// initial (static) text buffer
	int yytext_size;				// text buffer size
	int yystext_size;				// initial (static) text buffer size
	int yytext_max;					// maximum size of the text buffer

	// unput buffer
	yywint_t * yyunputbufptr;	// unput buffer
	yywint_t * yysunputbufptr;	// initial (static) unput buffer
	int yyunput_size;				// unput buffer size
	int yysunput_size;				// initial (static) unput buffer size
	int yyunputindex;				// unput buffer position
	int yyunput_max;				// maximum size of the unput buffer

	// actions
	yybool yymoreflg;				// concatenate matched strings
	yybool yyrejectflg;				// yyreject called from an action
	yybool yyreturnflg;				// return from an action
public:
	yywparser * yyparserptr;	// pointer to the attached parser

	// buffer flags
	yybool yytextgrow;				// whether text buffer is allowed to grow
	yybool yyunputgrow;				// whether unput buffer is allowed to grow

	// streams
#ifdef YYSTDCPPLIB
	YYSTD wistream* yyin;		// 70h input text stream
	YYSTD wostream* yyout;	        // 78h output text stream
	YYSTD wostream* yyerr;	        // 80h error stream
#else
	YYCSTD FILE* yyin;		// 70h input text stream
	YYCSTD FILE* yyout;		// 78h output text stream
	YYCSTD FILE* yyerr;		// 80h error stream
#endif

	// matched string
	wchar_t * yytext;			// text buffer
	int yyleng;						// matched string length
	int yylineno;					// current line number

// Operations
protected:
	// helper functions
	static yybool yyback(const yywmatch_t * p, int action);
public:
	// instance functions
	yybool yycreate(yywparser * parserptr = NULL);
	void yydestroy();

	// general functions
	void yycleanup();
	void yyreset();
	yybool yysettextsize(int size);
	yybool yysetunputsize(int size);

	virtual yywint_t yylex() = 0;       // vmt+08h

	// service functions
	virtual yywint_t yyinput();         // vmt+10h
	virtual void yyoutput(yywint_t ch); // vmt+18h
	virtual void yyunput(yywint_t ch);  // vmt+20h
	virtual int yywrap();               // vmt+28h
	virtual yywint_t yygetchar();       // vmt+30h
	virtual void yytextoverflow();      // vmt+38h
	virtual void yyunputoverflow();     // vmt+40h
	virtual yywint_t yyaction(int action) = 0;   // vmt+50h
/* virtual */ void yyerror(const wchar_t* text);  // has no record in vmt

	// action functions
	void yyecho();
	void yyless(int length);
	void yybegin(int state) { yystart = state; }
	void yymore() { yymoreflg = yytrue; }
	void yynewline(yybool newline) { newline ? yyeol = yytrue : (yyeol = yyfalse); }
	void yyreject() { yyrejectflg = yytrue; }
	int yyunputcount() const { return yyunputindex; }

	// compatibility
	yywint_t yyclex() { return yylex(); }
	void yylexcleanup() { yycleanup(); }
	void yylexinit() { /* do nothing */ }
	void yycleanuplexer() { yycleanup(); }
	void yyinitlexer() { /* do nothing */ }
#define BEGIN yystart = 
#define ECHO yyecho()
#define REJECT yyreject()
#define YYSTATE yystart
#define YY_START yystart

// Tables
protected:
	const yywmatch_t * yymatch; // 98h
	const yywstate_t * yystate;  // A0h
	const yywbackup_t * yybackup; // A8h

// Debugging
#ifdef YYDEBUG
public:
	yybool yydebug;						// whether debug information should be output
	yybool yydebugflush;				// whether debug output should be flushed
#ifdef YYSTDCPPLIB
	YYSTD wostream * yydebugout;	// debug output file
#else
	YYCSTD FILE * yydebugout;		// debug output file
#endif
protected:
	void yydebugoutput(yywint_t ch) const;
	void yydmatch(int expr) const;
	void yydebugoutput(const wchar_t * string) const;
#endif
};

class YYEXPCLASS  yywflexer : public yywlexer {
public:
	yywflexer() { /* do nothing */ }

// Operations
public:
	virtual yywint_t yylex();

// Tables
protected:
	const yywtransition_t * yytransition; // B0h
	long yytransitionmax;				  // B8h
};

_YL_END

// user defines
#if defined(YYBUDEFS) || defined(YYUDEFS)
#include <yybudefs.h>
#endif

// defines
#include "yybdefs.h"

#endif
