#include "parse.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include "rules.h"
#include "parser.hh"

#if 0
void yyparse();

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
#endif

#if 0
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
    
    // Parse up to a newline, then check if it's escaped, and if it is,
    // keep going
    while( true ) {
        newBufferOffset = inputBufferOffset;
        while( newBufferOffset < inputBufferSize && inputBuffer[newBufferOffset] != '\n') {
            newBufferOffset ++;
        }

        if( newBufferOffset != inputBufferSize
         && inputBuffer[ newBufferOffset - 1 ] != '\\' ) {
            yylval = new_string( inputBuffer + inputBufferOffset + 1,
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
#endif

#if 0
int yylex()
{
    // See if we have a line
    if( inputBufferOffset == inputBufferSize ) {
        // Read  new line
        int length;
        inputBufferOffset = 0;
        while( !feof( f ) ) {
            fgets(inputBuffer + inputBufferOffset, inputBufferMaxSize - inputBufferOffset, f);
            length = strlen( inputBuffer );
            if( length == 0 || inputBuffer[ length - 1 ] != '\n') {
                inputBufferOffset = inputBufferMaxSize - 1;
                inputBufferMaxSize *= 2;
                inputBuffer = (char *)realloc( inputBuffer, inputBufferMaxSize );
            } else {
                break;
            }
        }

        // If the first character is a tab, this is a command
        if( inputBuffer[0] == '\t' ) {
            yylval = (void *)inputBuffer + 1;
            return RULECOMMAND;
        }
    }
#if 0
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
#endif
    return ID;
}
#endif


