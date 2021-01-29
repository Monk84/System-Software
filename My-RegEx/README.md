# Laboratory work 2
Prepared by student Amir Kiamov
Variant 5
# Description
Escape sequence is %s%, where s - metasymbol  
Operations supported:
- or '|'
- concat 
- Kleene-star '*'
- optional part '?'
- repeat 'r{x}', where x - number of repeats, r - literal
- named capture group (<name>r), '<name>' - metasymbol, name - name of capture group, r means regex to capture
- capture group call <name>
  
Functions supported:
- search - returns first (return all captured groups too)
- typable symbols = [32..255]\{127}
