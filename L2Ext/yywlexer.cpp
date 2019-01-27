#include "StdAfx.h"

// namespaces
#ifdef YYSTDCPPLIB
using namespace std;
#endif
#ifdef YYNAMESPACE
using namespace yl;
#endif

int yywflexer::yylex(void){ return 0; }
void yywlexer::yyunputoverflow(void) {}
void yywlexer::yytextoverflow(void) {}
int yywlexer::yygetchar(void) { return 0; }
int yywlexer::yywrap(void) { return 0; }
void yywlexer::yyunput(int) {}
void yywlexer::yyoutput(int) {}
int yywlexer::yyinput(void) { return 0; }
yywlexer::~yywlexer(void) {}
yywlexer::yywlexer(void) {}
int yywparser::yygettoken(void) { return 0; }
void yywparser::yydiscard(int) {}
void yywparser::yysyntaxerror(void) {}
void yywparser::yyerror(wchar_t const *) {}
void yywparser::yystackoverflow(void) {}
void yywparser::yycopyattribute(void *,void const *,int) {}
void yywparser::yydeleteattribute(void *) {}
void * yywparser::yynewattribute(int) { return NULL; }
void yywparser::yylvaltostack(int) {}
void yywparser::yyvaltolval(void) {}
void yywparser::yylvaltoval(void) {}
void yywparser::yyvaltostack(int) {}
void yywparser::yystacktoval(int) {}
yywparser::~yywparser(void) {}
yywparser::yywparser(void) {}
void yywfparser::yydestructclearin(void) {}
int yywfparser::yywork(void) { return 0; }