%{
	#include"helper.c"
%}

delim		[ \t\n]
letter		[A-Za-z]
digit		[0-9]
id		{letter}({letter}|{digit})*
int		-?{digit}+
real  		{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
\n		{ line++;									}
[ \t]		{										}
"{"[^}]*"}"                 {line++; }
PROGRAM		{ return _PROGRAM ;								}
VAR		{ return _VAR ;									}
BEGIN		{ return _BEGIN ;								}
END		{ return _END ;									}
END\.		{ return _END_DOT ;								}
INTEGER		{ return _INTEGER ;								}
REAL		{ return _REAL ;								}
FOR		{ return _FOR ;									}
READ		{ return _READ;									}
WRITE		{ return _WRITE ;								}
TO		{ return _TO;									}
DO		{ return _DO;									}
\;		{ return _SEMICOLON;								}
\:		{ return _COLON;								}
\,		{ return _COMMA;								}
\:\=		{ return _ASSIGN;								}
\+		{ return _ADD;									}
\-		{ return _SUB;									}
\*		{ return _MUL;									}
DIV		{ return _DIV;									}
\(		{ return _OPEN_BRACE;								}
\)		{ return _CLOSE_BRACE;								}
{id}		{ yylval.ID = install_id();  return _ID;					}
{int}		{ yylval.NUMBER.integer = install_int(); yylval.NUMBER.type = 0; return _INT;	}
{real}		{ yylval.NUMBER.real = install_real(); yylval.NUMBER.type = 1; return _FLOAT;	}
.		{ printf("error: unexpected character syntax error, line number: %d\n", line + 1);	}
%%

char* install_id() {
	symbol* sym = search_in_hash_table(yytext);
	if(sym == NULL)
		sym = insert(yytext, 'V');
	return strdup(yytext);
}

int install_int() {
	return atoi(yytext);
}

float install_real() {
	return atof(yytext);
}