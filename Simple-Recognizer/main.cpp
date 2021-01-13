#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cctype>
#include "RegExSolution.h"
#include "FSMSolution.h"
#include <conio.h>

using namespace std;

std::string GetHash( const std::string& line )
{
	//assumed that string is valid, behavior is undefined for invalid string
	std::regex r( "[a-fA-F0-9]+(?=(\\|/))" );
	std::smatch m;
	std::regex_search( line,m,r );
	if ( m.length() < 1 )
	{
		return std::string( "" );
	}
	std::string result = m.str( 0 );
	for ( char& c : result )
	{
		c = static_cast<char>(std::tolower( static_cast<unsigned char>(c) ));
	}
	return result;
}

typedef bool (FSMSolution::*pCheck)(const std::string&);
FSMSolution fsm;
bool FSM_Check_String( const std::string& s )
{
	return fsm.CheckString(s);
}

int main( int argc,char* argv[] )
{
	if ( argc < 3 )
	{
		std::cout << "No arguments - starting with default arguments.\n";
	}
	bool type_fsm = true;
	bool type_input_console = true;
	bool type_output_console = true;
	std::ifstream ifile;
	std::ofstream ofile;
	FSMSolution fsm;

	for ( int i = 1; i < std::min<int>( argc,9 ); ++i ) // no more than 9 arguments
	{
		std::string arg = argv[i];
		if ( arg == "-t" )
		{
			if ( i + 1 < argc )
			{
				std::string etype = argv[i + 1];
				if ( etype == "reg" )
				{
					type_fsm = false;
				}
				else if ( !(etype == "fsm") )
				{
					std::cout << "Unrecognized execution type. Set to fsm.\n";
				}
				++i;
			}
		}
		else if ( arg == "-i" )
		{
			if ( i + 1 < argc )
			{
				std::string itype = argv[i + 1];
				if ( itype == "file" )
				{
					if ( i + 2 < argc )
					{
						ifile.open( argv[i + 2],std::ifstream::in );
						type_input_console = false;
						if ( !ifile )
						{
							std::cout << "Bad filename. Input set to console.\n";
							type_input_console = true;
						}
						++i;
					}
				}
				else if ( !(itype == "console") )
				{
					std::cout << "Unrecognized input type. Set to console.\n";
				}
				++i;
			}
		}
		else if ( arg == "-o" )
		{
			if ( i + 1 < argc )
			{
				std::string otype = argv[i + 1];
				if ( otype == "file" )
				{
					if ( i + 2 < argc )
					{
						ofile.open( argv[i + 2],std::ofstream::trunc );
						type_output_console = false;
						if ( !ofile )
						{
							std::cout << "Bad filename. Output set to console.\n";
							type_output_console = true;
						}
						++i;
					}
				}
				else if ( !(otype == "console") )
				{
					std::cout << "Unrecognized output type. Set to console.\n";
				}
				++i;
			}

		}
	}

	std::map<std::string,int> stats;
	bool( *exec_func )(const std::string& s);
	std::istream input( NULL );
	std::ostream output( NULL );
	if ( type_fsm )
	{
		exec_func = &FSM_Check_String;
	}
	else
	{
		exec_func = &RegExSolution;
	}
	if ( type_input_console )
	{
		input.rdbuf( std::cin.rdbuf() );
	}
	else
	{
		input.rdbuf( ifile.rdbuf() );
	}
	if ( type_output_console )
	{
		output.rdbuf( std::cout.rdbuf() );
	}
	else
	{
		output.rdbuf( ofile.rdbuf() );
	}

	if ( type_input_console )
	{
		std::cout << "Input line by line:\n";
	}

	while ( input.good() )
	{
		std::string line;
		std::getline( input,line );
		if ( type_input_console && line == "" )
		{
			break;
		}
		if ( (*exec_func)(line) )
		{
			std::string hash = GetHash( line );
			++stats[hash];
		}
	}
	output << "Statistics:\nHash - number'o'occurencies\n";
	for ( auto pair : stats )
	{
		output << std::get<0>( pair ) << " - " << std::get<1>( pair ) << std::endl;
	}
	_getch();
	return 0;
}
