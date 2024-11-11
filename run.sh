lex lexer.l
yacc -d compiler.y
cc lex.yy.c y.tab.c