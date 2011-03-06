%{
#define YYSTYPE void*
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "rules.h"
int yylex(void);
void yyerror(const char *s);
void print_rule( void *);
void * make_rule( void *, void *);
void * make_rule_header( void *, void *);
void * make_dependencies( void *, void *);
void * new_stringlist();
void * add_stringlist( void *, void *);

#define BASIC_LINE		10

static char bufferChar;
static char *inputBuffer;
static int inputBufferSize;
static unsigned int inputBufferMaxSize;
static int inputBufferOffset;
static FILE *f;

%}

%token RULECOMMAND
%token ID
%token END

%%

program: END
	| statement program
	;

statement:
	blankline
	| rule  					{ }

blankline:
	'\n'

rule:
	ruleheader rulebody			{ $$ = make_rule($1, $2); }
	;

ruleheader:
	targetlist ':' dependencies '\n' 	{ $$ = make_rule_header( $1, $3 ); }
	;

dependencies:
	sourcelist					{ $$ = make_dependencies($1, NULL); }
	| sourcelist '|' sourcelist { $$ = make_dependencies($1, $3); }
	;
	
rulebody:
	RULECOMMAND 				{ $$ = add_stringlist(new_stringlist(), $1); }
	| rulebody RULECOMMAND 		{ $$ = add_stringlist($1, $2); }
	;

targetlist:						{ $$ = new_stringlist(); }
	| targetlist ID 			{ $$ = add_stringlist($1, $2); }
	;

sourcelist:						{ $$ = new_stringlist(); }
	| sourcelist ID 			{ $$ = add_stringlist($1, $2); }
	;

%%
bool isspecial(char c)
{
	return strchr( "|:\n\t", c ) != NULL;
}

bool isidchar(char c)
{
	return !isspecial(c) && c != ' ';
}

void eatSpace()
{
	// Eat spaces from the beginning of the buffer
	while( inputBufferOffset < inputBufferSize && inputBuffer[inputBufferOffset] == ' ') {
		inputBufferOffset ++;
	}
}

int yylex()
{
	if( inputBufferOffset != inputBufferSize ) {
		inputBuffer[ inputBufferOffset ] = bufferChar;
	}

	eatSpace();

	// See if we have a line
	while( inputBufferOffset == inputBufferSize ) {
		// Read  new line
		inputBufferOffset = 0;
		if( feof(f) ) return END;
		while( !feof( f ) ) {
			fgets(inputBuffer + inputBufferOffset, inputBufferMaxSize - inputBufferOffset, f);
			inputBufferSize = strlen( inputBuffer );
			if( inputBufferSize == 0 || inputBuffer[ inputBufferSize - 1 ] != '\n') {
				inputBufferOffset = inputBufferMaxSize - 1;
				inputBufferMaxSize *= 2;
				inputBuffer = (char *)realloc( inputBuffer, inputBufferMaxSize );
			} else {
				break;
			}
		}
		inputBufferOffset = 0;

		// If the first character is a tab, this is a command
		if( inputBuffer[0] == '\t' ) {
			int length;
			yylval = (void *)(inputBuffer + 1);

			// Knock any carriage returns off the end
			length = strlen( inputBuffer ) - 1;
			while( length > 0 && strchr("\r\n", inputBuffer[ length ] )) inputBuffer[ length-- ] = 0;

			// Consume the whole string
			inputBufferOffset = inputBufferSize;
			return RULECOMMAND;
		}

		eatSpace();
	}

	// Choose the token type based on the first character
	if( isspecial( inputBuffer[ inputBufferOffset ] ) ) {
		bufferChar = inputBuffer[ inputBufferOffset + 1 ];
		return inputBuffer[ inputBufferOffset ++ ];
	} else {
		// It's an identifier. Keep parsing as long as we see alphanumerics
		int newBufferOffset = inputBufferOffset;
		while( newBufferOffset < inputBufferSize && isidchar(inputBuffer[ newBufferOffset ] ) ) {
			newBufferOffset ++;
		}

		yylval = inputBuffer + inputBufferOffset;
		bufferChar = inputBuffer[ newBufferOffset ];
		inputBuffer[ newBufferOffset ] = 0;
		inputBufferOffset = newBufferOffset;
		return ID;
	}
}

void parse_makefile( std::string filename )
{
	inputBufferOffset = BASIC_LINE;
	inputBufferSize = BASIC_LINE;
	inputBufferMaxSize = BASIC_LINE;
	inputBuffer = (char *)malloc( BASIC_LINE );
    if( inputBuffer == NULL ) {
        throw std::runtime_error( "Out of memory" );
    }
	f = fopen( filename.c_str(), "rb" );
	if( f == NULL ) {
		throw std::runtime_error( "Could not open makefile" );
	}

	yyparse();

	free( inputBuffer );
	fclose( f );
}

void yyerror(const char *s)
{
	throw std::runtime_error( "Parse error" );
}

void print_rule( void *rule)
{
	((Rule *)rule)->print();
}

void * make_rule( void *rule, void *commands)
{
	((Rule *)rule)->addCommandList( (std::list<std::string> *)commands );
	return rule;
}

void * make_rule_header( void *targets, void *dependencies)
{
	Rule *r = new Rule;

	r->addTargetList( (std::list<std::string> *)targets );
	return r;
}

void * make_dependencies( void *main, void *orderOnly)
{
	// We don't currently track dependencies. It may be useful to track them at
	// some point for debugging, but I never want to expect them
	delete (std::list<std::string> *)main;
	if( orderOnly != NULL ) {
		delete (std::list<std::string> *)orderOnly;
	}
	return NULL;
}

void * new_stringlist()
{
	return new std::list<std::string>;
}

void * add_stringlist( void *list, void *s)
{
	((std::list<std::string> *)list)->push_back((std::string)(char *)s);
	return list;
}
