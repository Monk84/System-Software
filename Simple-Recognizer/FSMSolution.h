#pragma once
#include "recognizer_sm.h"
#include <string.h>

class FSMSolution : public recognizerContext<FSMSolution>
{
private:
	recognizerContext fsm;
	bool isAcceptable;
	std::string buffer;
	int counter;
	const static std::string valid_begin;
public:
	inline FSMSolution() = default;
	
	~FSMSolution();

	bool CheckString( const std::string& s );

	inline void Acceptable()
	{
		isAcceptable = true;
	}

	inline void Unacceptable()
	{
		isAcceptable = false;
	}

	inline void CounterTick()
	{
		counter += 1;
	}

	inline int CounterState() const
	{
		return counter;
	}

	inline void ResetCounter()
	{
		counter = 0;
	}

	inline void BufferAdd( char c )
	{
		buffer.push_back( c );
	}

	inline bool BufferValidStatic() const
	{
		std::size_t found = valid_begin.find( buffer );
		return found != std::string::npos;
	}

	inline bool BufferFullStatic() const
	{
		return buffer == valid_begin;
	}

	inline void ResetBuffer()
	{
		buffer.clear();
	}
};
