#pragma once
#include "nfsm.h"
#include <iostream>

class DFSM
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
private:
	class Node
	{
	public:
		enum class TransitionType
		{
			Character,
			CaptureCall, //in nexts char is number of group
		};
	public:
		Node( bool isf = false,bool isd = false )
			:
			isfinish( isf ),
			isdeadlock( isd )
		{}
		void AddTransition( TransitionType t,char c,Node* n )
		{
			if ( !std::get<1>( nexts.emplace( std::pair<TransitionType,char>( t,c ),n ) ) )
			{
				throw ParsingException( "DFSM: error while adding transition" );
			}
		}
		bool IsFinish() const
		{
			return isfinish;
		}
		bool IsDeadlock() const
		{
			return isdeadlock;
		}
		Node* Transition( char a )
		{
			auto it = nexts.find( std::pair<TransitionType,char>( DFSM::Node::TransitionType::Character,a ) );
			if ( it != nexts.end() )
			{
				return it->second;
			}
			return nullptr;
		}
		~Node()
		{}
	private:
		std::map<std::pair<TransitionType,char>,Node*> nexts;
		bool isfinish;
		bool isdeadlock;
	};
public:
	DFSM( const NFSM& nfsm )
	{
		std::set<char> used_symbols;
		std::map<std::set<NFSM::Node*>,std::pair<DFSM::Node*,bool>> corresp_vertices;
		start = new Node();
		deadlock = new Node( false,true );
		nodes.emplace( start );
		nodes.emplace( deadlock );
		for ( auto node : nfsm.nodes )
		{
			for ( auto pr : node->nexts )
			{
				if ( pr.first.first == NFSM::Node::TransitionType::Character )
				{
					used_symbols.emplace( pr.first.second );
				}
			}
		}
		Node* curr_node = start;
		std::set<NFSM::Node*> eps_st = Epsilon_closure( { nfsm.start } );
		corresp_vertices.emplace( eps_st,std::pair<DFSM::Node*,bool>( start,false ) );
		auto it = std::find_if( corresp_vertices.begin(),corresp_vertices.end(),
			[]( std::pair<std::set<NFSM::Node*>,std::pair<DFSM::Node*,bool>> p )
			{
				return !p.second.second;
			}
		);
		while ( it != corresp_vertices.end() )
		{
			(it->second).second = true;
			for ( char a : used_symbols )
			{
				std::set<NFSM::Node*> m_a = M_x( it->first,a );
				if ( m_a.empty() )
				{
					continue;
				}
				m_a = Epsilon_closure( m_a );
				auto itt = corresp_vertices.find( m_a );
				if ( itt == corresp_vertices.end() )
				{
					bool isfin = false;
					if ( m_a.find( nfsm.finish ) != m_a.end() )
					{
						isfin = true;
					}
					Node* newnode = new Node( isfin );
					nodes.emplace( newnode );
					corresp_vertices.emplace( m_a,std::pair<DFSM::Node*,bool>( newnode,false ) );
					itt = corresp_vertices.find( m_a );
				}
				curr_node->AddTransition( Node::TransitionType::Character,a,(itt->second).first );
			}
			it = std::find_if( corresp_vertices.begin(),corresp_vertices.end(),
				[]( std::pair<std::set<NFSM::Node*>,std::pair<DFSM::Node*,bool>> p )
			{
				return !p.second.second;
			} );
			if ( it != corresp_vertices.end() )
			{
				curr_node = (it->second).first;
			}
		}
		currentNode = start;
	}
	void ToStart()
	{
		currentNode = start;
	}
	void Transition( char a )
	{
		auto nod = currentNode->Transition( a );
		if ( nod != nullptr )
		{
			currentNode = nod;
		}
		else
		{
			currentNode = deadlock;
		}
	}
	bool IsFinish() const
	{
		return currentNode->IsFinish();
	}
	bool IsDeadLock() const
	{
		return currentNode->IsDeadlock();
	}
	~DFSM()
	{
		for ( auto nod : nodes )
		{
			nod->~Node();
		}
	}
private:
	std::set<Node*> nodes;
	Node* start = nullptr;
	Node* deadlock = nullptr;
	Node* currentNode = nullptr;
};