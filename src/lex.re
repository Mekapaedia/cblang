#include "cbc.h"

#define RETINC (int)(YYCURSOR-YYSTART)
int NEXTSPEC = 0;
char **SPECIALTYPELIST = NULL;
int SPECIALTYPENUM = 0;

static int lexer(lexed *nextlex, const char *YYCURSOR);
void getintval(lexed *lexeme, const char *YYSTART, const char *YYCURSOR);
void getfloatval(lexed *lexeme, const char *YYSTART, const char *YYCURSOR);
void getstr(lexed *lexeme, const char *YYSTART, const char *YYCURSOR);
void getid(lexed *lexeme, const char *YYSTART, const char *YYCURSOR);
void gettype(lexed *lexeme, const char *YYSTART, const char *YYCURSOR);
char *convstrgen(const char *YYSTART, const char *YYCURSOR, char trim);

int lex(lexed *lexlist, char *input, size_t inputlen)
{
	unsigned int i = 0;
	int lexnum = 0;
	int lexret = 0;
	int character = 1;
	int line = 1;
	
	SPECIALTYPELIST = malloc(sizeof(char *) * 1);
	if(SPECIALTYPELIST == NULL)
	{
		perror(MEMALLOCERR);
		return -1;
	}
	for(i = 0; i < inputlen; i++)
	while(i < inputlen)
	{
		unsigned int newi = i;
		lexlist[lexnum].type = -1;
		lexlist[lexnum].typesize = -1;
		lexlist[lexnum].ptrlvl = -1;
		lexlist[lexnum].idname = NULL;
		lexlist[lexnum].val.strval = NULL;
		lexlist[lexnum].signedness = -1;
		lexlist[lexnum].fracsize = -1;
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
		
		type = "uf" [0-9]{1,3} "." [0-9]{1,3} | "sf" [0-9]{1,3} "." [0-9]{1,3} | "si" [0-9]{1,3} | "ui" [0-9]{1,3} | "bf" [0-9]{1,3} | "df" [0-9]{1,3} | "p" | "w" | "c" | "f" ;
		id = [a-zA-Z] [a-zA-Z0-9_-]*;
		int = "-"? [0-9]+;
		float = [0-9]+ "." [0-9]+;
		char = "'" "\\"? [a-zA-Z0-9]{1,1} "'";
		string = "\"" ([^"]|[\\] ["])* "\"";

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
		ws "$" ws { nextlex->lexeme = DLRSGN; return RETINC; }
		ws "if" ws { nextlex->lexeme = IF; return RETINC; }
		ws "elif" ws { nextlex->lexeme = ELIF; return RETINC; }
		ws "else" ws { nextlex->lexeme = ELSE; return RETINC; }
		ws "while" ws { nextlex->lexeme = WHILE; return RETINC; }
		ws "return" ws { nextlex->lexeme = RET; return RETINC; }
		ws "break" ws { nextlex->lexeme = BREAK; return RETINC; }
		ws "continue" ws { nextlex->lexeme = CONTINUE; return RETINC; }
		ws "struct" ws { nextlex->lexeme = STRUCT; NEXTSPEC = 1; return RETINC; }
		ws "union" ws { nextlex->lexeme = UNION; NEXTSPEC = 1; return RETINC; }
		ws "=" ws { nextlex->lexeme = ASSIGN; return RETINC; }
		ws type ws { nextlex->lexeme = TYPE; gettype(nextlex, YYSTART, YYCURSOR); return RETINC; }
		ws id ws { nextlex->lexeme = ID; getid(nextlex, YYSTART, YYCURSOR); return RETINC; }
		ws int ws { nextlex->lexeme = INT; getintval(nextlex, YYSTART, YYCURSOR); return RETINC; }
		ws float ws { nextlex->lexeme = FLOAT; getfloatval(nextlex, YYSTART, YYCURSOR); return RETINC; }
		ws char ws { nextlex->lexeme = CHAR; getstr(nextlex, YYSTART, YYCURSOR); return RETINC; }
		ws string ws  { nextlex->lexeme = CHAR; getstr(nextlex, YYSTART, YYCURSOR); return RETINC; }
	*/
}

char *convstrgen(const char *YYSTART, const char *YYCURSOR, char trim)
{
	const char *strcur = YYSTART;
	char *convstr = NULL;
	int i = 0;
	
	if(YYSTART == NULL || YYCURSOR == NULL)
	{
		return NULL;
	}
	convstr = malloc(sizeof(char)*(size_t)(YYCURSOR-YYSTART+1));
	if(convstr == NULL)
	{
		perror(MEMALLOCERR);
		return NULL;
	}
	if(trim == 2)
	{
		strcur++;
	}
	while(strcur < YYCURSOR)
	{
		if((trim == 1) && (*strcur == ' ' || *strcur == '\n' || *strcur == '\r' || *strcur == '\t' || *strcur == '\v' || *strcur == '\f'))
		{
			strcur++;
			continue;
		}
		convstr[i] = *strcur;
		strcur++;
		i++;
	}
	convstr[i] = '\0';
	if(trim == 2)
	{
		convstr[i-1] = '\0';
	}
	return convstr;
}

void getintval(lexed *lexeme, const char *YYSTART, const char *YYCURSOR)
{
	char *convstr = NULL;

	convstr = convstrgen(YYSTART, YYCURSOR, 1);
	if(convstr == NULL)
	{
		return;
	}
	lexeme->signedness = 1;
	lexeme->val.intval = strtol(convstr, NULL, 10);
	free(convstr);
}

void getfloatval(lexed *lexeme, const char *YYSTART, const char *YYCURSOR)
{
	char *convstr = NULL;

	convstr = convstrgen(YYSTART, YYCURSOR, 1);
	if(convstr == NULL)
	{
		return;
	}
	lexeme->val.floatval = strtod(convstr, NULL);
	free(convstr);

}

void getstr(lexed *lexeme, const char *YYSTART, const char *YYCURSOR)
{
	char *convstr = NULL;

	convstr = convstrgen(YYSTART, YYCURSOR, 2);
	if(convstr == NULL)
	{
		return;
	}
	lexeme->val.strval = convstr;
}

void getid(lexed *lexeme, const char *YYSTART, const char *YYCURSOR)
{
	char *convstr = NULL;
	int i = 0;

	convstr = convstrgen(YYSTART, YYCURSOR, 1);
	if(convstr == NULL)
	{
		return;
	}
	lexeme->idname = convstr;
	if(NEXTSPEC == 1)
	{
		lexeme->type = SP;
		SPECIALTYPELIST[SPECIALTYPENUM] = convstr;
		SPECIALTYPELIST = realloc(SPECIALTYPELIST, sizeof(char *) * (SPECIALTYPENUM + 1));
		if(SPECIALTYPELIST == NULL)
		{
			perror(MEMALLOCERR);
			return;
		}
		SPECIALTYPENUM++;
		NEXTSPEC = 0;
	}
	else
	{
		for(i = 0; i < SPECIALTYPENUM; i++)
		{
			if(!strcmp(convstr, SPECIALTYPELIST[i]))
			{
				lexeme->type = SP;
				lexeme->lexeme = TYPE;
				free(convstr);
				lexeme->idname = SPECIALTYPELIST[i];
				break;		
			}
		}
	}
}

void gettype(lexed *lexeme, const char *YYSTART, const char *YYCURSOR)
{
	const char *strcur = YYSTART;
	while((*strcur == ' ' || *strcur == '\n' || *strcur == '\r' || *strcur == '\t' || *strcur == '\v' || *strcur == '\f') && strcur < YYCURSOR)
	{
		strcur++;
	}
	if(strcur == YYCURSOR)
	{
		return;
	}
	if(*strcur == 'p')
	{
		lexeme->type = P;
		lexeme->typesize = PSIZE;
	}
	else if(*strcur == 'w')
	{
		lexeme->type = W;
		lexeme->typesize = WSIZE;
	}
	else if(*strcur == 'c')
	{
		lexeme->type = C;
		lexeme->typesize = CSIZE;
	}
	else if(*strcur == 'f')
	{
		lexeme->type = F;
		lexeme->typesize = FSIZE;
	}
	else if(*strcur == 'v')
	{
		lexeme->type = V;
	}
	else if((*strcur == 's' || *strcur == 'u') && *(strcur+1) == 'f')
	{
		char *convstri = NULL;
		char *convstrf = NULL;
		const char *dotcur = strcur+2;
		while(dotcur < YYCURSOR)
		{
			if(*dotcur == '.')
			{
				break;
			}
			dotcur++;
		}
		
		convstri = convstrgen(strcur+2, dotcur, 1);
		if(convstri == NULL)
		{
			return;
		}
		convstrf = convstrgen(dotcur+1, YYCURSOR, 1);
		if(convstrf == NULL)
		{
			return;
		}

		lexeme->typesize = strtol(convstri, NULL, 10);
		lexeme->fracsize = strtol(convstrf, NULL, 10);
		
		if(*strcur == 's')
		{
			lexeme->type = SF;
		}
		else if(*strcur == 'u')
		{
			lexeme->type = UF;
		}
		free(convstri);
		free(convstrf);
	}
	else
	{
		char *convstr = NULL;
		convstr = convstrgen(strcur+2, YYCURSOR, 1);
		if(convstr == NULL)
		{
			return;
		}
		if(*strcur == 's' && *(strcur+1) == 'i')
		{
			lexeme->type = SI;
			lexeme->typesize = strtol(convstr, NULL, 10);
		}
		else if(*strcur == 'u' && *(strcur+1) == 'i')
		{

			lexeme->type = UI;
			lexeme->typesize = strtol(convstr, NULL, 10);
		}
		else if(*strcur == 'b' && *(strcur+1) == 'f')
		{

			lexeme->type = BF;
			lexeme->typesize = strtol(convstr, NULL, 10);
		}
		else if(*strcur == 'd' && *(strcur+1) == 'f')
		{

			lexeme->type = DF;
			lexeme->typesize = strtol(convstr, NULL, 10);
		}
		free(convstr);
	}
}
/* vim: set ft=c: */
