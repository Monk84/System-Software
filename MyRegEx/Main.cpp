/*********LABORATORY WORK 2*********
Prepared by student Amir Kiamov
Variant 5
Escape sequence is %s%, where s - metasymbol
Operations supported:
or '|'
concat 
Kleene-star '*'
optional part '?'
repeat 'r{x}', where x - number of repeats, r - literal
named capture group (<name>r), '<name>' - metasymbol, name - name of capture group, r means regex to capture
capture group call <name>

Functions supported:
search - returns first (return all captured groups too)
typable symbols = [32..255]\{127}
*/

#include <iostream>
#include <Windows.h>
#include "MyRegEx.h"

int main( int argc,char* argv )
{
	{
		//test 1
		std::string s = "a?1";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "1123";
		int res = myregex->Search( test );
		if ( res == 0 )
		{
			std::cout << "Test 1 passed successfully";
		}
		else
		{
			std::cout << "Test 1 failed";
		}
	}
	{
		//test 2
		std::string s = "aaaa";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "1123172849573728189395969959483828183949";
		int res = myregex->Search( test );
		if ( res == -1 )
		{
			std::cout << "\nTest 2 passed successfully";
		}
		else
		{
			std::cout << "\nTest 2 failed";
		}
	}
	{	
		//test 3
		std::string s = "aaaa";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "112317aaa28495737aaa28189395aaaa969959483828183949aaaa";
		int res = myregex->Search( test );
		if ( res == 28 )
		{
			std::cout << "\nTest 3 passed successfully";
		}
		else
		{
			std::cout << "\nTest 3 failed";
		}
	}
	{
		//test 4
		std::string s = "a{2}";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "11aaaaaaaaaaaa23172849573728189395969959483828183949";
		int res = myregex->Search( test );
		if ( res == 2 )
		{
			std::cout << "\nTest 4 passed successfully";
		}
		else
		{
			std::cout << "\nTest 4 failed";
		}
	}
	{
		//test 5
		std::string s = "a+b+c+d+";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "aaaaaaaaabbbbbbbcccd";
		int res = myregex->Search( test );
		if ( res == 0 )
		{
			std::cout << "\nTest 5 passed successfully";
		}
		else
		{
			std::cout << "\nTest 5 failed";
		}
	}
	{
		//test 6
		std::string s = "a+b+|c+d+";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "aaaaaaaaabbbbbbbcccd";
		int res = myregex->Search( test );
		if ( res == 0 )
		{
			std::cout << "\nTest 6 passed successfully";
		}
		else
		{
			std::cout << "\nTest 6 failed";
		}
	}
	{
		//test 7
		std::string s = "a+b+|c+d+";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "aaaaaaaaa9bbbbbbbcccd";
		int res = myregex->Search( test );
		if ( res == test.find_first_of('c') )
		{
			std::cout << "\nTest 7 passed successfully";
		}
		else
		{
			std::cout << "\nTest 7 failed";
		}
	}
	{
		//test 8
		std::string s = "a*b*|(c+d+)";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "bbbbbbbcccd";
		int res = myregex->Search( test );
		if ( res == 0 )
		{
			std::cout << "\nTest 8 passed successfully";
		}
		else
		{
			std::cout << "\nTest 8 failed";
		}
	}
	{
		//test 9
		std::string s = "%%%a*b*|(c+d+)";
		MyRegEx* myregex = nullptr;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			std::cout << e.GetErrorMessage();
		}
		std::string test = "%bbbbbbbcccd";
		int res = myregex->Search( test );
		if ( res == 0 )
		{
			std::cout << "\nTest 9 passed successfully";
		}
		else
		{
			std::cout << "\nTest 9 failed";
		}
	}
	{
		//test 10
		std::string s = "%%(?%a*b*|(c+d+)";
		MyRegEx* myregex = nullptr;
		bool res = false;
		try
		{
			myregex = new MyRegEx( s );
		}
		catch ( const MyRegEx::ParsingException& e )
		{
			//std::cout << e.GetErrorMessage();
			std::cout << "\nTest 10 passed successfully\n";
			res = true;
		}
		if( !res )
			std::cout << "\nTest 10 failed\n";
	}
	system( "pause" );
	return 0;
}
