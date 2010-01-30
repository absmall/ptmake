%{
	int yylex(void);
%}
%token ID
%token RULECOMMAND

%%

program:
	| statement program
	;

statement:
	blankline
	| rule

blankline:
	'\n'

rule:
	ruleheader rulebody			{ setNormalMode(); }
	;

ruleheader:
	targetlist ':' dependencies '\n' 	{ setCommandMode(); }
	;

dependencies:
	sourcelist
	| sourcelist '|' sourcelist
	
rulebody:
	RULECOMMAND 
	| rulebody RULECOMMAND ;

targetlist:
	| targetlist ID ;

sourcelist:
	| sourcelist ID ;
