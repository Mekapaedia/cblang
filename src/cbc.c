#include "cbc.h"

int main(int argc, char **argv) 
{
	if(argc < 2)
	{
		help(basename(argv[0]));
	}
	else
	{
		if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "-H") || !strcmp(argv[1], "--help"))
		{
			help(basename(argv[0]));
		}
		else if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "-V") || !strcmp(argv[1], "--version"))
		{
			ver();
		}
		else if(argv[1][0] != '-')
		{
			FILE *file = NULL;
			size_t filesize = 0;
			char *buf = NULL;
			size_t amtread = 0;
			size_t buflen = 0;
			lexed *lexedlist = NULL;
			int lexret = 0;
			int i = 0;

			file = fopen(argv[1], "r");
			if(file == NULL)
			{
				perror("File could not be opened");
				return 1;
			}
			fseek (file, 0, SEEK_END);
			filesize = (size_t) ftell(file);
			rewind(file);
			buflen = sizeof(char)*filesize + 1;
			buf = (char*) malloc(buflen);
			memset(buf, '\0', sizeof(char)*filesize + 1);
			if(buf == NULL)
			{
				perror("Memory allocation error");
				return 1;
			}
			
			amtread = fread(buf,1,filesize,file);
			if(amtread != filesize)
			{
				perror("File read error");
				return 1;
			}
			
			fclose(file);			
			printf("%s\n", buf);

			lexedlist = malloc(sizeof(lexed)*filesize);
			if(lexedlist == NULL)
			{
				perror("Memory allocation error");
				return 1;
			}

			lexret = lex(lexedlist, buf, buflen);
			free(buf);
			
			if(lexret < 0)
			{
				fprintf(stderr, "Lexing error");
				free(lexedlist);
				return 1;
			}

			for(i = 0; i < lexret; i++)
			{
				lexprint(&lexedlist[i]);
			}

			free(lexedlist);
		}
		else
		{
			help(basename(argv[0]));
		}
	}

	return 0;
}

void help(const char *name)
{
	if(!name)
	{
		name = "cbc";
	}
	ver();
	puts("");
	printf("Usage: %s file.cb\n", name);
}

void ver(void)
{
	printf("Cb Compiler Version %s\n", CBC_VER);
	printf("Cb Language Version %s\n", CBLANG_VER);
}

void lexprint(lexed *lexeme)
{
	if(lexeme->lexeme == UNKNOWN)
	{
		puts("ERR: UNKNOWN");
	}
	else if(lexeme->lexeme == EOTEXT)
	{
		puts("EOTEXT");
	}
	else if(lexeme->lexeme == PERIOD) 
	{
		puts("PERIOD"); 
	}
	else if(lexeme->lexeme == SMICLN)
	{
		 puts("SMICLN");
	}
	else if(lexeme->lexeme == CLN)
	{
		puts("CLN");
	}
	else if(lexeme->lexeme == COMMA)
	{
		puts("COMMA");
	}
	else if(lexeme->lexeme == RBRAC)
	{
		puts("RBRAC");
	}
	else if(lexeme->lexeme == LBRAC)
	{
		puts("LBRAC");
	}
	else if(lexeme->lexeme == RPAR)
	{
		puts("RPAR");
	}
	else if(lexeme->lexeme == LPAR)
	{
		puts("LPAR");
	}
	else if(lexeme->lexeme == RSRQ)
	{
		puts("RSRQ");
	}
	else if(lexeme->lexeme == LSRQ)
	{
		puts("LSRQ");
	}
	else if(lexeme->lexeme == ARROW)
	{
		puts("ARROW");
	}
	else if(lexeme->lexeme == MINUS)
	{
		puts("MINUS");
	}
	else if(lexeme->lexeme == PLUS)
	{
		puts("PLUS");
	}
	else if(lexeme->lexeme == MOD)
	{
		puts("MOD");
	}
	else if(lexeme->lexeme == STAR)
	{
		puts("STAR");
	}
	else if(lexeme->lexeme == DIV)
	{
		puts("DIV");
	}
	else if(lexeme->lexeme == EXCL)
	{
		puts("EXCL");
	}
	else if(lexeme->lexeme == AMP)
	{
		puts("AMP");
	}
	else if(lexeme->lexeme == TILDE)
	{
		puts("TILDE");
	}
	else if(lexeme->lexeme == OR)
	{
		puts("OR");
	}
	else if(lexeme->lexeme == AND)
	{
		puts("AND");
	}
	else if(lexeme->lexeme == XOR)
	{
		puts("XOR");
	}
	else if(lexeme->lexeme == PIPE)
	{
		puts("PIPE");
	}
	else if(lexeme->lexeme == CARET)
	{
		puts("CARET");
	}
	else if(lexeme->lexeme == RSHFT)
	{
		puts("RSHFT");
	}
	else if(lexeme->lexeme == RROT)
	{
		puts("RROT");
	}
	else if(lexeme->lexeme == LSHFT)
	{
		puts("LSHFT");
	}
	else if(lexeme->lexeme == LROT)
	{
		puts("LROT");
	}
	else if(lexeme->lexeme == EQ)
	{
		puts("EQ");
	}
	else if(lexeme->lexeme == NE)
	{
		puts("NE");
	}
	else if(lexeme->lexeme == GT)
	{
		puts("GT");
	}
	else if(lexeme->lexeme == GE)
	{
		puts("GE");
	}
	else if(lexeme->lexeme == LT)
	{
		puts("LT");
	}
	else if(lexeme->lexeme == LE)
	{
		puts("LE");
	}
	else if(lexeme->lexeme == IF)
	{
		puts("IF");
	}
	else if(lexeme->lexeme == ELIF)
	{
		puts("ELIF");
	}
	else if(lexeme->lexeme == ELSE)
	{
		puts("ELSE");
	}
	else if(lexeme->lexeme == WHILE)
	{
		puts("WHILE");
	}
	else if(lexeme->lexeme == RET)
	{
		puts("RET");
	}
	else if(lexeme->lexeme == STRUCT)
	{
		puts("STRUCT");
	}
	else if(lexeme->lexeme == UNION)
	{
		puts("UNION");
	}
	else if(lexeme->lexeme == ASSIGN)
	{
		puts("ASSIGN");
	}
	else if(lexeme->lexeme == TYPE)
	{
		puts("TYPE");
	}
	else if(lexeme->lexeme == ID)
	{
		puts("IDENT");
	}
	else if(lexeme->lexeme == TYPE)
	{
		puts("TYPE");
	}
	else if(lexeme->lexeme == INT)
	{
		puts("INT");
	}
	else if(lexeme->lexeme == FLOAT)
	{
		puts("FLOAT");
	}
	else if(lexeme->lexeme == CHAR)
	{
		puts("CHAR");
	}
	else
	{
		puts("ERR: UUNKNOWN");
	}
}
