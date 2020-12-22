#pragma once
#include "SyntaxTree.h"

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
	class Node
	{
	public:
		enum class TransitionType
		{
			Epsilon,
			Character,
			Start,
		};
	public:
		Node( SyntaxTree::Node* nod )
		{
			if ( nod->children.size() == 0 && (nod->type != SyntaxTree::NodeType::Leaf ||
				nod->type != SyntaxTree::NodeType::OperatorCaptureCall) )
			{
				throw ParsingException( "NFSM: found operator node with no successors" );
			}
			for ( auto succ : nod->children )
			{
								
			}
		}
	private:
		std::vector<Node*> nexts;
		bool isfinish;
		TransitionType transtype;
		char trans; //if type is epsilon then trans = 0
	};
public:
	NFSM( const SyntaxTree& st )
	{
		start = new Node( st.root );
		for ( auto capt : st.capturegroups )
		{

		}
	}
	//maybe here i can merge based on treenode type...
private:
	Node* start;
	std::vector<Node*> captures;
};
