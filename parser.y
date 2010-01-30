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
	ruleheader				{ setNormalMode() }
	| ruleheader rulebody			{ setNormalMode() }
	;

ruleheader:
	targetlist ':' sourcelist '\n' 		{ setRuleMode() }
	;

rulebody:
	RULECOMMAND 
	| rulebody RULECOMMAND ;

targetlist:
	| targetlist ID ;

sourcelist:
	| sourcelist ID ;
