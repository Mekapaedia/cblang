#include "cbc.h"

#define RETINC (int)(YYCURSOR-YYSTART)

static int lexer(lexed *nextlex, const char *YYCURSOR);

int lex(lexed *lexlist, char *input, size_t inputlen)
{
	unsigned int i = 0;
	int lexnum = 0;
	int lexret = 0;
	int character = 1;
	int line = 1;

	for(i = 0; i < inputlen; i++)
	while(i < inputlen)
	{
		unsigned int newi = i;
		lexret = lexer(&lexlist[lexnum], &input[i]);
		lexlist[lexnum].line = line;
		lexlist[lexnum].character = character;
		lexnum++;
		if(lexret < 0)
		{
			unsigned int j = 0;
			char symb[70];
			for(j = 0; j < (unsigned int)-lexret; j++)
			{
				symb[j] = input[i + j];
			}
			symb[j] = '\0';
			fprintf(stderr, "Lexing error: Unknown symbol %s at line %d character %d\n", symb, line, character);
			return -1;
		}
		newi = i + (unsigned int) lexret;
		while(i < newi)
		{
			if(input[i] == '\n')
			{
				line++;
				character = 1;
			}
			else
			{
				character++;
			}
			i++;
		}
	}
	return lexnum;
}

static int lexer(lexed *nextlex, const char *YYCURSOR)
{
	const char *YYMARKER;
	const char *YYSTART = YYCURSOR;
	/*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

		ws = [\n\r\t\v\f ]*;
		end = "\x00";
		
		type = "si" [0-9]{1,3} | "ui" [0-9]{1,3} | "bf" [0-9]{1,3} | "df" [0-9]{1,3} | "p" | "w" | "c" | "f" ;
		id = [a-zA-Z] [a-zA-Z0-9_-]*;
		int = [0-9]+;
		float = [0-9]+ "." [0-9]+;
		char = "'" [a-zA-Z] "'" | "'" "\\" [a-zA-Z0-9] "'";
		string = "\"" [^"]* "\"";

		* { nextlex->lexeme = UNKNOWN; return -RETINC; }
		end	{ nextlex->lexeme = EOTEXT; return RETINC; }
		ws "." ws { nextlex->lexeme = PERIOD; return RETINC; }
		ws ";" ws { nextlex->lexeme = SMICLN; return RETINC; }
		ws ":" ws { nextlex->lexeme = CLN; return RETINC; }
		ws "," ws { nextlex->lexeme = COMMA; return RETINC; }
		ws "}" ws { nextlex->lexeme = RBRAC; return RETINC; }
		ws "{" ws { nextlex->lexeme = LBRAC; return RETINC; }
		ws ")" ws { nextlex->lexeme = RPAR; return RETINC; }
		ws "(" ws { nextlex->lexeme = LPAR; return RETINC; }
		ws "]" ws { nextlex->lexeme = RSRQ; return RETINC; }
		ws "[" ws { nextlex->lexeme = LSRQ; return RETINC; }
		ws "->" ws { nextlex->lexeme = ARROW; return RETINC; }
		ws "-" ws { nextlex->lexeme = MINUS; return RETINC; }
		ws "+" ws { nextlex->lexeme = PLUS; return RETINC; }
		ws "%" ws { nextlex->lexeme = MOD; return RETINC; }
		ws "*" ws { nextlex->lexeme = STAR; return RETINC; }
		ws "/" ws { nextlex->lexeme = DIV; return RETINC; }
		ws "!" ws { nextlex->lexeme = EXCL; return RETINC; }
		ws "&" ws { nextlex->lexeme = AMP; return RETINC; }
		ws "~" ws { nextlex->lexeme = TILDE; return RETINC; }
		ws "||" ws { nextlex->lexeme = OR; return RETINC; }
		ws "&&" ws { nextlex->lexeme = AND; return RETINC; }
		ws "^^" ws { nextlex->lexeme = XOR; return RETINC; }
		ws "|" ws { nextlex->lexeme = PIPE; return RETINC; }
		ws "^" ws { nextlex->lexeme = CARET; return RETINC; }
		ws ">>" ws { nextlex->lexeme = RSHFT; return RETINC; }
		ws ">)" ws { nextlex->lexeme = RROT; return RETINC; }
		ws "<<" ws { nextlex->lexeme = LSHFT; return RETINC; }
		ws "(<" ws { nextlex->lexeme = LROT; return RETINC; }
		ws "==" ws { nextlex->lexeme = EQ; return RETINC; }
		ws "!=" ws { nextlex->lexeme = NE; return RETINC; }
		ws ">" ws { nextlex->lexeme = GT; return RETINC; }
		ws ">=" ws { nextlex->lexeme = GE; return RETINC; }
		ws "<" ws { nextlex->lexeme = LT; return RETINC; }
		ws "<=" ws { nextlex->lexeme = LE; return RETINC; }
		ws "if" ws { nextlex->lexeme = IF; return RETINC; }
		ws "elif" ws { nextlex->lexeme = ELIF; return RETINC; }
		ws "else" ws { nextlex->lexeme = ELSE; return RETINC; }
		ws "while" ws { nextlex->lexeme = WHILE; return RETINC; }
		ws "return" ws { nextlex->lexeme = RET; return RETINC; }
		ws "struct" ws { nextlex->lexeme = STRUCT; return RETINC; }
		ws "union" ws { nextlex->lexeme = UNION; return RETINC; }
		ws "=" ws { nextlex->lexeme = ASSIGN; return RETINC; }
		ws type ws { nextlex->lexeme = TYPE; return RETINC; }
		ws id ws { nextlex->lexeme = ID; return RETINC; }
		ws int ws { nextlex->lexeme = INT; return RETINC; }
		ws float ws { nextlex->lexeme = FLOAT; return RETINC; }
		ws char ws { nextlex->lexeme = CHAR; return RETINC; }
		ws string ws  { nextlex->lexeme = CHAR; return RETINC; }
	*/
}

/* vim: set ft=c: */
