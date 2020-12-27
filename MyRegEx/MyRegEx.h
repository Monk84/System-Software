#pragma once

#include <string>
#include "SyntaxTree.h"
#include "NFSM.h"
#include "DFSM.h"

class MyRegEx
{
public:
	class ParsingException
	{
	public:
		ParsingException( const std::string& msg )
			:
			msg( msg )
		{}
		const std::string& GetErrorMessage() const
		{
			return msg;
		}
	private:
		std::string msg;
	};
public:
	MyRegEx( const std::string& s )
	{
		SyntaxTree* st = nullptr;
		NFSM* nfsm = nullptr;
		innerAutomata = nullptr;
		try
		{
			st = new SyntaxTree( s );
		}
		catch ( SyntaxTree::ParsingException& e )
		{
			throw ParsingException( e.GetErrorMessage() );
		}
		if ( st != nullptr )
		{
			try
			{
				nfsm = new NFSM( *st );
			}
			catch ( NFSM::ParsingException& e )
			{
				throw ParsingException( e.GetErrorMessage() );
			}
		}
		if ( nfsm != nullptr )
		{
			try
			{
				innerAutomata = new DFSM( *nfsm );
			}
			catch ( DFSM::ParsingException& e )
			{
				throw ParsingException( e.GetErrorMessage() );
			}
		}
		if ( innerAutomata == nullptr )
		{
			throw ParsingException( "MyRegEx: error while building regex" );
		}
	}
	MyRegEx( const MyRegEx& ) = delete;
	MyRegEx& operator=( const MyRegEx& ) = delete;
	~MyRegEx()
	{
		innerAutomata->~DFSM();
	}
	int Search( const std::string& s )
	{
		innerAutomata->ToStart();
		int index = 0;
		for ( int i = 0; i < s.size(); ++i )
		{
			index = i;
			int run = index;
			while ( !innerAutomata->IsFinish() && !innerAutomata->IsDeadLock() && run != s.size() )
			{
				innerAutomata->Transition( s[run] );
				++run;
			}
			if ( innerAutomata->IsFinish() )
			{
				return index;
			}
			innerAutomata->ToStart();
		}
		return -1;
	}
private:
	DFSM* innerAutomata;
};