#############
cl-calculator
#############

========
Overview
========

Command Line Calculator

Solves Following Operations: + - / * ^

Solves Paren ()

Supoorts double numbers and negatives

Extras:

K is converted to ``*2^10``

M is converted to ``*2^20``

G is converted to ``*2^30``
    
IE ``2K - 1`` is converted to ``2*1024 - 1``

=============
Compile Steps
=============

.. code::

    $ gcc calc.c -lm -o calc

============
Sample Usage
============

.. code::

    $ ./calc 2+3
    2+3=5
  
    # Must use quotes with Paren
    $ ./calc '(2+3) - (24-1.5)'
