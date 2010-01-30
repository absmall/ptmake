%{
	int yylex(void);
%}
%token ID
%token RULECOMMAND

%%

program:
	rule 					{ printf("Rule parsed\n"); }
	;

rule:
	ruleheader rulebody			{ setNormalMode(); }
	;

ruleheader:
	targetlist ':' sourcelist '\n' 		{ setCommandMode(); }
	;

rulebody:
	RULECOMMAND 
	| rulebody RULECOMMAND ;

targetlist:
	| targetlist ID ;

sourcelist:
	| sourcelist ID ;
