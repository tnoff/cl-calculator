cl-calculator
=============

Command Line Calculator

Solves Following Operations: + - / * ^

Solves Paren ()

Supoorts double numbers and negatives

Extras:

K is converted to '*1024'

M is converted to *1024*1024'

G is converted to '*1024*1024*1024'
    
IE '2K - 1' is converted to '2*1024 - 1'

Compile Steps:
    gcc calc.c -lm -o calc

Sample Usage:
    ./calc 2+3
  
  Must use quotes with Paren
    ./calc '(2+3) - (24-1.5)'
  
