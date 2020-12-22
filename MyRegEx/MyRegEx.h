#pragma once

#include <string>
#include "SyntaxTree.h"
#include "NFSM.h"

class MyRegEx
{
public:
	MyRegEx() = delete;
	MyRegEx( const std::string& s );
	MyRegEx( const MyRegEx& ) = delete;
	MyRegEx& operator=( const MyRegEx& ) = delete;
	~MyRegEx();
	std::_String_iterator<std::string>& Search( const std::string& s );
private:
	SyntaxTree& BuildTree( const std::string& s )
	{
		return SyntaxTree( s );
	}
	NFSM& TreeToNFSM( const SyntaxTree& st );
	//DFSM& NtoD( const NFSM& nfsm ); //returns minimized DFSM
private:
	//DFSM innerAutomata;
};
