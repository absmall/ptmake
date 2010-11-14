#include "parse.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include "rules.h"
extern "C" {
#include "parser.h"
}

enum Mode{
	Normal,
	Command
};

static Mode mode;
static char *inputBuffer;
static int inputBufferSize;
static int inputBufferOffset;
static FILE *f;

extern "C" {
void yyparse();

void setNormalMode()
{
	mode = Normal;
}

void setCommandMode()
{
	mode = Command;
}

bool refillBuffer()
{
	int newdata;

	// See if the buffer is already full. If so, we have to make it bigger.
	if( inputBufferOffset == 0 ) {
		char *newBuf;
		newBuf = (char *)realloc( inputBuffer, inputBufferSize + 80 );
		if( !newBuf ) {
			throw std::runtime_error( "Out of memory resizing parsing buffer" );
		}
		inputBuffer = newBuf;
		newdata = fread( inputBuffer + inputBufferSize, 1, 80, f );
		inputBufferSize += newdata;
	} else {
		// The buffer isn't full, so just try to refill it
		memmove(inputBuffer, inputBuffer+inputBufferOffset, inputBufferSize - inputBufferOffset);

		// Try to refill the buffer
		newdata = fread( inputBuffer + inputBufferSize - inputBufferOffset, 1, inputBufferOffset, f );
		if( newdata < inputBufferOffset ) {
			// We couldn't refill everything, so shrink the buffer
			inputBufferSize -= inputBufferOffset - newdata;
		}
		inputBufferOffset = 0;
	}

	// return whether or not we read anything
	return newdata != 0;
}

bool isspecial(char c)
{
	return strchr( "|:\n\t", c ) != NULL;
}

bool isidchar(char c)
{
	return !isspecial(c) && c != ' ';
}

int normalParse()
{
	// Eat spaces
	while( true ) {
		while( inputBufferOffset < inputBufferSize && inputBuffer[inputBufferOffset] == ' ') {
			inputBufferOffset ++;
		}

		// If we dont have enough data, refill the buffer
		if( inputBufferOffset == inputBufferSize ) {
			if( !refillBuffer() ) {
				return 0;
			}
		} else {
			break;
		}
	}

	// Choose the token type based on the first character
	if( isspecial( inputBuffer[ inputBufferOffset ] ) ) {
		return inputBuffer[ inputBufferOffset ++ ];
	} else {
		// It's an identifier. Keep parsing as long as we see alphanumerics
		int newBufferOffset = inputBufferOffset;
		while( true ) {
			while( newBufferOffset < inputBufferSize && isidchar(inputBuffer[ newBufferOffset ] ) ) {
				newBufferOffset ++;
			}

			// See if we found a termination in the buffer.
			if( newBufferOffset == inputBufferSize ) {
				if( !refillBuffer() ) {
					// There's nothing more, just return the token
					inputBufferOffset = newBufferOffset;
					return 0;
				}
			} else {
				break;
			}
		}

		yylval.s = new_string( inputBuffer + inputBufferOffset,
                               inputBuffer + newBufferOffset );
		inputBufferOffset = newBufferOffset;
		return ID;
	}
}

int commandParse()
{
	int newBufferOffset;

	// Commands must start with a tab, and go until a newline that
	// is not escaped
	if( inputBufferOffset == inputBufferSize ) {
		if( !refillBuffer() ) {
			return 0;
		}
	}
	if( inputBuffer[ inputBufferOffset ] != '\t' ) {
		// It's not a rule. Go to normal parser.
		setNormalMode();
		return normalParse();
	}
	
	// Parse up to a newline, then check if it's escaped, and if it is,
	// keep going
	while( true ) {
		newBufferOffset = inputBufferOffset;
		while( newBufferOffset < inputBufferSize && inputBuffer[newBufferOffset] != '\n') {
			newBufferOffset ++;
		}

		if( newBufferOffset != inputBufferSize
		 && inputBuffer[ newBufferOffset - 1 ] != '\\' ) {
            yylval.s = new_string( inputBuffer + inputBufferOffset + 1,
                                   inputBuffer + newBufferOffset );
			inputBufferOffset = newBufferOffset + 1;
			return RULECOMMAND;
		}

		if( !refillBuffer() ) {
            yylval.s = new_string( inputBuffer + inputBufferOffset,
                                   inputBuffer + newBufferOffset );
			return RULECOMMAND;
		}
	}
}

int yylex()
{
	int ret;

	switch( mode ) {
		case Normal:
			ret = normalParse();
			break;
		case Command:
			ret = commandParse();
			break;
		default:
			throw std::runtime_error("Internal error: Unexpected parser mode");
	}
	return ret;
}

void yyerror(char *s)
{
	throw std::runtime_error( "Parse error" );
}
}

void parse_makefile( std::string filename )
{
	mode = Normal;
	inputBufferOffset = 0;
	inputBufferSize = 0;
	inputBuffer = (char *)malloc( 1 );
    if( inputBuffer == NULL ) {
        throw std::runtime_error( "Out of memory" );
    }
	f = fopen( filename.c_str(), "r" );
	if( f == NULL ) {
		throw std::runtime_error( "Could not open makefile" );
	}

	yyparse();

	free( inputBuffer );
	fclose( f );
}
