/** @file Debug.hpp
* Contains functions useful for debugging, like asserts.
* @author Keeler Russell
* @date Oct 13, 2011
*/
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <cstdlib>
#include <string>

/****************************************
* Debug Flags
****************************************/
// Change this to DEBUG_MESSAGES_OFF to turn them off
#define DEBUG_MESSAGES_ON
// Change this to ASSERTIONS_OFF to turn them off
#define ASSERTIONS_ON

/***************************************
* Assertion Macros
***************************************/
#ifdef ASSERTIONS_ON
/* Asserts the condition, and terminates the program if false.
* @param X The boolean condition statement to test.
* @pre X must be an expression which can evaluate to true or false. ASSERTIONS_ON must be defined.
* @post Program execution will terminate if the condition X is false.
* The condition, the filename, and the line number will be printed before exiting.
*/
#define fatalAssert( X ) if( !( X ) ) { std::cout << "Fatal assertion failed: " << #X << ", File: " << __FILE__ << ", Line: " << __LINE__ << std::endl; exit( -1 ); }
/* Asserts the condition, but does not terminate the program if false.
* @param X The boolean condition statement to test.
* @pre X must be an expression which can evaluate to true or false. ASSERTIONS_ON must be defined.
* @post If the condition X is false, the condition, the filename, and the line number will be printed.
*/
#define softAssert( X ) if( !( X ) ) { std::cout << "Soft assertion failed: " << #X << ", File: " << __FILE__ << ", Line: " << __LINE__ << std::endl; }
/* A fatal assert that always occurs if executed.
* @pre ASSERTIONS_ON must be defined.
* @post Program execution will terminate; the filename, and line number will be printed before exiting.
*/
#define alwaysAssert() { throw( -1 ); }
#else
#define fatalAssert( X ) {}
#define softAssert( X ) {}
#define alwaysAssert() {}
#endif

/****************************************
* Function Prototypes
****************************************/
/** Prints a message to standard output.
* @param message The message to print.
* @pre DEBUG_MESSAGES_ON must be defined.
* @post The message will be printed to standard output.
*/
void printDebugMessage( const std::string & message );
/** Prints a message to standard output before terminating program execution.
* @param message The message to print before exiting.
* @pre None.
* @post The message will be printed to standard output, then the program will terminate.
*/
void fatalError( const std::string & message );

#endif

