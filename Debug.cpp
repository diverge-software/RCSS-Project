#include "Debug.hpp"

using std::cout;
using std::cerr;
using std::endl;

void printDebugMessage( const std::string & message )
{
	#ifdef DEBUG_MESSAGES_ON
		cout << message << endl << endl;
	#endif
}

void fatalError( const std::string & message )
{
	cerr << message << endl;
	exit( 1 );
}

