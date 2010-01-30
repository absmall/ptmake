%{
#include <stdlib.h>
#include "rules.h"
int yylex(void);
void yyerror(char *s);
void setNormalMode();
void setCommandMode();
%}

%union {
    Rule *rule;
    RuleHeader *ruleHeader;
    Dependencies *dependencies;
    StringList *sl;
    String *s;
}

%token <s>ID
%token <s>RULECOMMAND
%type  <rule>rule
%type  <ruleHeader>ruleheader
%type  <sl>rulebody
%type  <sl>targetlist
%type  <sl>sourcelist
%type  <dependencies>dependencies
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
