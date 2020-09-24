#include <regex>
#include <string>

std::regex r( "^ed2k://\\|file\\|[\\w.+-]+\\|[\\d]+\\|[0-9a-fA-F]+\\|/$" );
//^ - beginning of string, $ - end of string, \w = [a-zA-Z0-9_], \d = [0-9]

bool RegExSolution( const std::string& s )
{
    return std::regex_match( s,r );
}
