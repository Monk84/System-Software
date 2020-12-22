/*********LABORATORY WORK 2*********
Prepared by student Amir Kiamov
Variant 5
Escape sequence is %s%, where s - metasymbol
Operations supported:
or '|'
concat '.' (optional)
Kleene-star '*'
optional part '?'
repeat 'r{x}', where x - number of repeats, r - literal
named capture group (<name>r), '<name>' - metasymbol, name - name of capture group, r means regex to capture
capture group call <name>

Functions supported:
search - returns first
*/

#include <iostream>
#include <Windows.h>
#include "MyRegEx.h"

int main( int argc,char* argv )
{
	/*std::string s = "fgtr(g)(ftgh(gtdeds(df(d))dfr)d)";
	std::vector<smallnode> newstring;
	for ( char c : s )
	{
		newstring.push_back( smallnode{ false,c } );
	}
	auto result = GetNearestParentheses( newstring );
	std::cout << s << " " << result.first << " " << result.second;*/
	/*std::string s = "(cfvgbhh<frgt>fhhf)";
	int p = 8;
	std::string name = "";
	while ( s[p] != '>' )
	{
		s.erase( s.begin() + p );
		name.push_back( s[p] );
	}
	//std::cout << s;
	std::cout << name;*/
	std::string s = "a+(g|f)*(<123>)+<123>(<1234>r|a)";
	try
	{
		SyntaxTree st( s );
	}
	catch ( SyntaxTree::ParsingException& e )
	{
		std::cout << e.GetErrorMessage();
	}
	//st.Print();
	//NFSM sm( st );
	system( "pause" );
	return 0;
}