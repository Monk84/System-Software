#pragma once
#include <map>
#include <set>
#include <stack>
#include "SyntaxTree.h"
#define NOMINMAX

class NFSM
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
	class Node
	{
	public:
		enum class TransitionType
		{
			Epsilon,
			Character,
			CaptureCall,
		};
	public:
		Node( bool isf )
			:
			isfinish( isf )
		{}
		Node( SyntaxTree::Node* nod )
			:
			isfinish( false )
		{
			if ( nod->children.size() == 0 && (nod->type != SyntaxTree::NodeType::Leaf ||
				nod->type != SyntaxTree::NodeType::OperatorCaptureCall) )
			{
				throw ParsingException( "NFSM: found operator node with no successors" );
			}
		}
		/*Node( std::map<Node*,Node*> m,Node* n )
		{
			isfinish = n->isfinish;
			for ( auto pr : n->nexts )
			{
				auto it = m.find( pr.second );
				if ( it == m.end() )
				{
					m.emplace( pr.second );
					AddTransition( pr.first.first,pr.first.second,new Node( m,pr.second ) );
				}
				else
				{
					AddTransition( pr.first.first,pr.first.second,it->second );
				}
			}
		}*/
		Node( Node* n )
		{
			isfinish = n->isfinish;
			for ( auto mv : n->nexts )
			{
				nexts.emplace( mv );
			}
		}
		void AddTransition( TransitionType t,char c,Node* n )
		{
			nexts.emplace( std::pair<TransitionType,char>( t,c ),n );
		}
		~Node()
		{
			printf( "node destructor\n" );
		}
	public:
		std::map<std::pair<TransitionType,char>,NFSM::Node*> nexts;
		bool isfinish;
		friend class DFSM;
		friend std::set<Node*> Epsilon_closure( const std::set<Node*>& n );
		friend std::set<Node*> M_x( const std::set<Node*>& n,char a );
	};
public:
	NFSM( SyntaxTree::Node* n )
	{
		if ( !start )
		{
			start = new Node( false );
			nodes.insert( start );
		}
		if ( !finish )
		{
			finish = new Node( true );
			nodes.insert( finish );
		}
		switch ( n->children.size() )
		{
		case 0:
		{
			if ( n->type == SyntaxTree::NodeType::OperatorCaptureCall )
			{
				start->AddTransition( NFSM::Node::TransitionType::CaptureCall,n->text,finish );
			}
			else // n type is Leaf
			{
				start->AddTransition( NFSM::Node::TransitionType::Character,n->text,finish );
			}
		}
		break;
		case 1: // unary operator
		{
			NFSM* child = new NFSM( n->children[0] );
			MergeUnary( n,*this,*child );
		}
		break;
		case 2: //binary operator
		{
			NFSM* child0 = new NFSM( n->children[0] );
			NFSM* child1 = new NFSM( n->children[1] );
			MergeBinary( n,*this,*(child0),*(child1) );
		}
		break;
		default:
		{
			throw ParsingException( "NFSM: ST node has three or more successors" );
		}
		}
	}
	NFSM( const SyntaxTree& st ) : NFSM( st.root )
	{
		for ( auto capt : st.capturegroups )
		{
			NFSM* child = new NFSM( capt );
			captures.push_back( child );
		}
	}
	NFSM( const NFSM& other )
	{
		for ( auto thing : other.captures )
		{
			captures.push_back( new NFSM( *thing ) );
		}
		std::map<NFSM::Node*,NFSM::Node*> nodess;
		start = new Node( other.start );
		nodes.emplace( start );
		nodess.emplace( std::pair<Node*,Node*>( other.start,start ) );
		finish = new Node( other.finish );
		nodes.emplace( finish );
		nodess.emplace( std::pair<Node*,Node*>( other.finish,finish ) );
		for ( Node* n : other.nodes )
		{
			if ( nodess.find( n ) == nodess.end() )
			{
				Node* newnode = new Node( n );
				nodes.emplace( newnode );
				nodess.emplace( std::pair<Node*,Node*>( n,newnode ) );
			}
		}
		for ( Node* n : nodes )
		{
			for ( auto& pr : n->nexts )
			{
				auto it = nodess.find( pr.second );
				if ( it != nodess.end() )
				{
					pr.second = it->second;
				}
			}
		}
	}
	NFSM& operator=( const NFSM& other ) = delete;
	~NFSM()
	{
		for ( auto node : nodes )
		{
			node->~Node();
		}
		for ( auto capture : captures )
		{
			capture->~NFSM();
		}
	}
	//friend void Merge( SyntaxTree::Node* n );
	friend void MergeUnary( SyntaxTree::Node* n,NFSM& main,NFSM& nfsm );
	friend void MergeBinary( SyntaxTree::Node* n,NFSM& main,NFSM& nfsm1,NFSM& nfsm2 );
	friend std::set<Node*> Epsilon_closure( const std::set<Node*>& n );
	friend std::set<Node*> M_x( const std::set<Node*>& n,char a );
private:
	Node* start = nullptr;
	Node* finish = nullptr;
	std::vector<NFSM*> captures;
	std::set<NFSM::Node*> nodes;
	friend class DFSM;
};

void MergeUnary( SyntaxTree::Node* n,NFSM& main,NFSM& nfsm )
{
	for ( auto n : nfsm.nodes )
	{
		main.nodes.emplace( n );
	}
	nfsm.finish->isfinish = false;
	main.start->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),nfsm.start );
	switch ( n->type )
	{
	case SyntaxTree::NodeType::OperatorKleene:
	{
		nfsm.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
		main.start->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 1 ),main.finish ); //my poor realization
		nfsm.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 1 ),nfsm.start );
	}
	break;
	case SyntaxTree::NodeType::OperatorPlus:
	{
		nfsm.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
		nfsm.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 1 ),nfsm.start );
	}
	break;
	case SyntaxTree::NodeType::OperatorOptional:
	{
		nfsm.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
		main.start->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 1 ),main.finish ); //my poor realization
	}
	break;
	case SyntaxTree::NodeType::OperatorRepeat:
	{
		int mint = (int( n->text ) < 255) ? int( n->text ) : 255;
		NFSM* n0 = &nfsm;
		NFSM* newnfsm = nullptr;
		for ( int i = 1; i < mint; ++i ) //we use char, it's 8 bit long (2**8 - 1 = 255)
		{
			newnfsm = new NFSM( nfsm );
			for ( auto nn : newnfsm->nodes )
			{
				main.nodes.emplace( nn );
			}
			n0->finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),newnfsm->start );
			n0 = newnfsm;
		}
		newnfsm->finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
	}
	break;
	default:
	{
		throw NFSM::ParsingException( "NFSM: ST node has one successor, but invalid type" );
	}
	break;
	}
}

void MergeBinary( SyntaxTree::Node* n,NFSM& main,NFSM& nfsm1,NFSM& nfsm2 )
{
	for ( auto n : nfsm1.nodes )
	{
		main.nodes.emplace( n );
	}
	for ( auto n : nfsm2.nodes )
	{
		main.nodes.emplace( n );
	}
	nfsm1.finish->isfinish = false;
	nfsm2.finish->isfinish = false;
	main.start->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),nfsm1.start );
	nfsm2.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
	switch ( n->type )
	{
	case SyntaxTree::NodeType::OperatorConcat:
	{
		nfsm1.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),nfsm2.start );
		break;
	}
	case SyntaxTree::NodeType::OperatorOr:
	{
		nfsm1.finish->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 0 ),main.finish );
		main.start->AddTransition( NFSM::Node::TransitionType::Epsilon,char( 1 ),nfsm2.start );
		break;
	}
	default:
	{
		throw NFSM::ParsingException( "NFSM: ST node has two successors, but invalid type" );
	}
	}
}

std::set<NFSM::Node*> Epsilon_closure( const std::set<NFSM::Node*>& n )
{
	std::stack<NFSM::Node*> st;
	std::set<NFSM::Node*> result;
	for ( auto node : n )
	{
		st.push( node );
		result.emplace( node );
	}
	while ( !st.empty() )
	{
		NFSM::Node* x = st.top();
		st.pop();
		for ( auto trans : x->nexts )
		{
			if ( trans.first.first == NFSM::Node::TransitionType::Epsilon )
			{
				NFSM::Node* nod = trans.second;
				if ( result.find( nod ) == result.end() )
				{
					st.push( nod );
					result.emplace( nod );
				}
			}
		}
	}
	return result;
}

std::set<NFSM::Node*> M_x( const std::set<NFSM::Node*>& n,char x )
{
	std::set<NFSM::Node*> result;
	for ( auto node : n )
	{
		for ( auto next : node->nexts )
		{
			if ( next.first.first == NFSM::Node::TransitionType::Character
				&& next.first.second == x )
			{
				result.emplace( next.second );
			}
		}
	}
	return result;
}
