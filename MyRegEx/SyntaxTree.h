#pragma once
#include <string>
#include <algorithm>
#include <vector>

/*TODO list
-- add opportunity of empty caprure group, thus, it captures only one symbol instead of regex
-- add repetition operator
-- add optionality operator
*/

class SyntaxTree
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
	enum class NodeType 
	{
		None,
		OperatorOr,
		OperatorConcat,
		OperatorKleene,
		OperatorPlus,
		OperatorOptional,
		OperatorRepeat, //this time ch is number
		OperatorCapture, //this time ch is group id
		OperatorCaptureCall, 
		Leaf,
		LeafCaprure, //if named group captures only one symbol, we have to remember it
	};
	class Node
	{
	public:
		char text;
		NodeType type;
		friend class NFSM;
	public:
		Node( char c,NodeType t,Node* n = nullptr )
			:
			text( c ),
			type( t ),
			parent( n )
		{}
		inline void AddChild( Node* pNode )
		{
			children.push_back( pNode );
		}
		inline void AddParent( Node* pNode )
		{
			if ( parent == nullptr )
			{
				parent = pNode;
			}
		}
		inline NodeType GetType() const
		{
			return type;
		}
		inline void Print() const
		{
			std::cout << "<";
			switch ( type )
			{
			case NodeType::None:
			{
				std::cout << "None";
				break;
			}
			case NodeType::OperatorCapture:
			{
				std::cout << "OperatorCapture";
				break;
			}
			case NodeType::OperatorCaptureCall:
			{
				std::cout << "OperatorCaptureCall";
				break;
			}
			case NodeType::OperatorKleene:
			{
				std::cout << "OperatorClini";
				break;
			}
			case NodeType::OperatorConcat:
			{
				std::cout << "OperatorConcat";
				break;
			}
			case NodeType::OperatorOptional:
			{
				std::cout << "OperatorOptional";
				break;
			}
			case NodeType::OperatorOr:
			{
				std::cout << "OperatorOr";
				break;
			}
			case NodeType::OperatorPlus:
			{
				std::cout << "OperatorPlus";
				break;
			}
			case NodeType::OperatorRepeat:
			{
				std::cout << "OperatorRepeat";
				break;
			}
			case NodeType::Leaf:
			{
				std::cout << "Leaf";
				break;
			}
			}
			std::cout << ", " << int( text ) << " " << text << "[";
			for ( auto p : children )
			{
				p->Print();
			}
			std::cout << "]>";
		}
	private:
		Node* parent;
		std::vector<Node*> children;
	};
public:
	SyntaxTree( const std::string& s );
	void Print() const;
	template<typename container,typename registry>
	friend void InitializeCaptureGroup( container& cont,int beg,int& end,registry& reg,bool initialize );
	friend class NFSM;
private:
	Node* root;
	std::vector<Node*> capturegroups;
};

template<typename container>
std::pair<int,int> GetNearestParentheses( const container& cont )
{
	int beg = 0,end = int(cont.size()),beg2 = 0;
	for ( int i = 1; i < cont.size(); ++i )
	{
		if ( cont[i]->text == '(' )
		{
			beg2 = i;
		}
		if ( cont[i]->text == ')' )
		{
			if ( i - beg2 < end - beg )
			{
				beg = beg2;
				end = i;
			}
		}
	}
	return std::pair<int,int>( beg,end );
}

template<typename container,typename registry>
void InitializeCaptureGroup( container& cont,int beg,int& end,registry& reg,bool initialize ) //beg is at '<', end is one before ')'
{
	std::string name = "";
	while ( cont[beg]->text != '>' && beg != end )
	{
		if(  )
		cont.erase( cont.begin() + beg );
		end -= 1;
		name.push_back( cont[beg]->text );
	}
	if ( beg == end )
	{
		throw SyntaxTree::ParsingException( "Capture groups: expected '>' at the end of name" );
	}
	name.pop_back();
	int id;
	if ( initialize ) //first occurrence of capture group with that name
	{
		if ( std::find( reg.begin(),reg.end(),name ) != reg.end() )
		{
			throw SyntaxTree::ParsingException( "Capture group: attempt to rewrite defined capture group" );
		}
		id = int(reg.size());
		reg.push_back( name );
		cont[beg]->type = SyntaxTree::NodeType::OperatorCapture;
		cont[beg]->text = id;
		if ( beg + 1 == end )
		{
			end += 1;
			cont.insert( cont.begin() + beg + 1,new Node( SyntaxTree::NodeType::LeafCaprure,0 ) );
		}
	}
	else //usage of capture group
	{
		auto it = std::find( reg.begin(),reg.end(),name );
		id = int(std::distance( reg.begin(),it ));
		cont[beg]->type = SyntaxTree::NodeType::OperatorCaptureCall;
		cont[beg]->text = id;
	}
}

SyntaxTree::SyntaxTree( const std::string& s )
{
	auto ss = s;
	ss.push_back( ')' );
	ss.insert( 0,1,'(' );
	std::vector<Node*> newstring = {};
	std::vector<std::string> captures = {};
	std::vector<char> operators = { '|','*','+','.','?' };
	for ( char c : ss )
	{
		newstring.push_back( new Node{ c,NodeType::None } );
	}
	while ( newstring.size() != 1 )
	{
		std::pair<int,int> nearest = GetNearestParentheses( newstring );
		//precheck for capture group init
		if ( newstring[nearest.first + 1]->text == '<' )
		{
			InitializeCaptureGroup( newstring,nearest.first + 1,nearest.second,captures,true );
		}
		//first iteration over string - plain nodes, capture groups and symbols to repeat are captured, operators are skipped
		for ( int i = nearest.first + 1; i < nearest.second; ++i )
		{
			if ( newstring[i]->type == NodeType::None )
			{
				char temp = newstring[i]->text;
				//ecraned symbols
				if ( temp == '%' )
				{
					if ( i + 2 < nearest.second )
						if ( newstring[i + 2]->text == '%' && newstring[i + 1]->type == NodeType::None )
						{
							nearest.second -= 2;
							newstring.erase( newstring.begin() + i + 2 );
							newstring.erase( newstring.begin() + i );
							newstring[i]->type = NodeType::Leaf;
							continue;
						}
				}
				if ( std::find( operators.begin(),operators.end(),temp ) != operators.end() )
				{
					continue;
				}
				//capture group
				if ( temp == '<' )
				{
					InitializeCaptureGroup( newstring,i,nearest.second,captures,false );
					continue;
				}
				newstring[i]->type = NodeType::Leaf;
			}
		}
		//second iteration - operators 
		for ( int i = nearest.first + 1; i < nearest.second; ++i )
		{
			if ( newstring[i]->type == NodeType::None )
			{
				char temp = newstring[i]->text;
				switch ( temp )
				{
				case '*':
					newstring[i]->type = NodeType::OperatorKleene;
					nearest.second -= 1;
					newstring[i]->AddChild( newstring[i - 1] );
					newstring.erase( newstring.begin() + i - 1 );
					i -= 1;
					break;
				case '?':
					newstring[i]->type = NodeType::OperatorOptional;
					nearest.second -= 1;
					newstring[i]->AddChild( newstring[i - 1] );
					newstring.erase( newstring.begin() + i - 1 );
					i -= 1;
					break;
				case '+':
					newstring[i]->type = NodeType::OperatorPlus;
					nearest.second -= 1;
					newstring[i]->AddChild( newstring[i - 1] );
					newstring.erase( newstring.begin() + i - 1 );
					i -= 1;
					break;
				}
			}
		}
		//third iteration: adding operator CONCAT
		for ( int i = nearest.first + 1; i < nearest.second; ++i )
		{
			if ( newstring[i - 1]->type != NodeType::None &&  newstring[i - 1]->type != NodeType::OperatorCapture
				&& newstring[i]->type != NodeType::None )
			{
				Node* concat = new Node( 0,NodeType::OperatorConcat );
				nearest.second -= 1;
				concat->AddChild( newstring[i - 1] );
				concat->AddChild( newstring[i] );
				newstring[i] = concat;
				newstring.erase( newstring.begin() + i - 1 );
				i -= 1;
			}
		}
		//fourth iteration: adding operator OR
		for ( int i = nearest.first + 1; i < nearest.second - 1; ++i )
		{
			if ( newstring[i]->type == NodeType::None && newstring[i]->text == '|' )
			{
				if ( newstring[i - 1]->type == NodeType::OperatorCapture )
				{
					throw SyntaxTree::ParsingException( "Operator OR is placed immediately after capture group init" );
				}
				newstring[i]->type = NodeType::OperatorOr;
				nearest.second -= 2;
				newstring[i]->AddChild( newstring[i - 1] );
				newstring[i]->AddChild( newstring[i + 1] );
				newstring.erase( newstring.begin() + i + 1 );
				newstring.erase( newstring.begin() + i - 1 );
				i -= 1;
			}
		}
		//fifth iteration: capture group activation
		if ( newstring[nearest.first + 1]->type == NodeType::OperatorCapture )
		{
			newstring[nearest.first + 1]->AddChild( newstring[nearest.first + 2] );
			newstring.erase( newstring.begin() + nearest.first + 2 );
			capturegroups.push_back( newstring[nearest.first + 1] );
			newstring[nearest.first + 1] = new Node( newstring[nearest.first + 1]->text,NodeType::OperatorCaptureCall );
			nearest.second -= 1;
		}
		//sixth iteration: eating parentheses
		if ( nearest.second - nearest.first != 2 )
		{
			throw ParsingException( "Fifth iteration - more than one node left betweeen parentheses" );
		}
		newstring.erase( newstring.begin() + nearest.second );
		newstring.erase( newstring.begin() + nearest.first );
	}
	root = newstring[0];
}

void SyntaxTree::Print() const
{
	root->Print();
}