/******************************************************************************

                              Laboratory work 1
Student: Amir Kiamov
Variant: 23
Task: develop simple recognizer of ed2k file links based on: 
1) regex 
2) finite state machine made with SMC

Make CLI interface with both file and console input possibility, after
execution output statistics for correct lines based on hash value into both file 
or console.

Syntax: sr [-t [fsm|reg]] [-i [console|file [filename]]] [-o [console|file [filename]]]
-t - type of solution (default fsm)
reg = RegEx
fsm = finite state machine
-i(o) - input (output) type (default console)
console - console input (output)
file - file input (ouput)
when choosing "file", specify filename by address
*******************************************************************************/

#include <iostream>
#include <fstream>
#include "RegExSolution.h"
#include "FSMSolution.h"

using namespace std;

int main( int argc,char* argv[] )
{
  bool type_fsm = True;
  bool type_input_console = True;
  bool type_output_console = True;
    if( argc > 1 )
      switch( *(argv[1]+1) ):
      case 't'
      {
        
      }
      if( *(argv[1]+1) == 't')
        if( argv[2] && *argv[2] == '' )
          type_fsm = False;
      
    return 0;
}
