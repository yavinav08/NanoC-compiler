# Assignment4
Avinav Yadav
210101025

# LEXICAL ANALYSIS & PARSER

The above code is a lexical analyzer in Flex and C programming language. It has been designed to parse a simple programming language and build a symbol table along with it and to check any errors in the syntax

# To run:
lex lexer.l
yacc -d compiler.y
cc lex.yy.c y.tab.c
./a.out < input.pas

The above implementation has been tested on MacOS

# Output
SYMTAB.txt :- Contains the hashtable of the symbols as linked lists.
All the errors are printed on the console

# DEFINITIONS:
1. SIZE is the number of buckets in the hash table.
2. symbol is a symbol in the hash table with specifier value and the type of the symbol and next pointer to point to the next entry in the hash table.


## All the flows and routines have been clearly explained in the code

