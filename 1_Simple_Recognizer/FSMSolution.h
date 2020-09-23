#include "FSMSolution_sm.h"

class FSMSolution : public FSMSOlutionContext<FSMSolution>
{
private:
	FSMSOlutionContext fsm;
	bool acceptable;
public:
	FSMSolution()
		:
		acceptable(False)
	{}
	~FSMSolution()
	{}
	inline bool CheckString(const std::string& s)
	{
		if( s.size() > 13 && s.substr(0,12).compare("ed2k://|file|") == 0 )
			for( char c : s.substr( 13,s.size()-1 )
			{
				try
				{
					fsm.symb(c);
				}
				catch( ... )
				{
					return false;
				}
			}
		return acceptable;
	}
	inline void Acceptable()
	{
		acceptable = True;
	}
	inline void Unacceptable()
	{
		acceptable = False;
	}
}
