%{
#include <stdlib.h>
#include <rules.h>
#include <stringlist.h>
int yylex(void);
void yyerror(char *s);
void setNormalMode();
void setCommandMode();
%}

%union {
    void *opaque;
}

%token  <opaque>ID
%token  <opaque>RULECOMMAND
%type <opaque>rule
%type <opaque>ruleheader
%type <opaque>rulebody
%type <opaque>targetlist
%type <opaque>sourcelist
%type <opaque>dependencies
%%

program:
	| statement program
	;

statement:
	blankline
	| rule  					{ print_rule( $1 ); }

blankline:
	'\n'

rule:
	ruleheader rulebody			{ setNormalMode(); $$ = make_rule($1, $2); }
	;

ruleheader:
	targetlist ':' dependencies '\n' 	{ setCommandMode(); $$ = make_rule_header( $1, $3 ); }
	;

dependencies:
	sourcelist				{ $$ = make_dependencies($1, NULL); }
	| sourcelist '|' sourcelist 		{ $$ = make_dependencies($1, $3); }
	;
	
rulebody:
	RULECOMMAND 				{ $$ = add_stringlist(new_stringlist(), $1); }
	| rulebody RULECOMMAND 			{ $$ = add_stringlist($1, $2); }
	;

targetlist:					{ $$ = new_stringlist(); }
	| targetlist ID 			{ $$ = add_stringlist($1, $2); }
	;

sourcelist:					{ $$ = new_stringlist(); }
	| sourcelist ID 			{ $$ = add_stringlist($1, $2); }
	;
