#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <cstdlib>
#include <string>

// Change this to DEBUG_MESSAGES_OFF to turn them off
#define DEBUG_MESSAGES_ON
// Change this to ASSERTIONS_OFF to turn them off
#define ASSERTIONS_ON

#ifdef ASSERTIONS_ON
// Asserts, then exits
#define fatalAssert( X ) if( !( X ) ) { std::cout << "Fatal assertion failed: " << #X <<  ", File: " \
                         << __FILE__ << ", Line: " << __LINE__ << std::endl << std::endl; exit( -1 ); }
// Asserts, but doesn't exit
#define softAssert( X ) if( !( X ) ) { std::cout << "Soft assertion failed: " << #X <<  ", File: " \
                         << __FILE__ << ", Line: " << __LINE__ << std::endl << std::endl; }
// Always asserts false, then exits
#define alwaysAssert() { std::cout << "Always assert here: " <<  ", File: " \
                          << __FILE__ << ", Line: " << __LINE__ << std::endl << std::endl; exit( -1 ) }
#else
#define fatalAssert( X ) {}
#define softAssert( X ) {}
#define alwaysAssert() {}
#endif


void printDebugMessage( const std::string & message );
void fatalError( const std::string & message );

#endif
