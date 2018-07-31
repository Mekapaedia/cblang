#ifndef _CBC_H_
#define _CBC_H_

#define CBC_VER "0.0.0"
#define CBLANG_VER "0.0.0"
#define PSIZE 64
#define WSIZE 64
#define FSIZE 32
#define CSIZE 8

#define MEMALLOCERR "Memory allocation error"

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include "parse.h"

enum lexemes
{
	EOTEXT,
	TYPE,
	ID,
	INT,
	FLOAT,
	CHAR,
	PERIOD,
	SMICLN,
	CLN,
	COMMA,
	RBRAC,
	LBRAC,
	RPAR,
	LPAR,
	RSRQ,
	LSRQ,
	ARROW,
	MINUS,
	PLUS,
	MOD,
	STAR,
	DIV,
	EXCL,
	AMP,
	TILDE,
	OR,
	AND,
	XOR,
	PIPE,
	CARET,
	RSHFT,
	RROT,
	LSHFT,
	LROT,
	EQ,
	NE,
	GT,
	GE,
	LT,
	LE,
	IF,
	ELIF,
	ELSE,
	WHILE,
	RET,
	STRUCT,
	UNION,
	ASSIGN,
	DLRSGN,
	BREAK,
	CONTINUE,
	ROOT,
	NONE,
	UNKNOWN	
};

enum types
{
	SI,
	UI,
	BF,
	DF,
	SF,
	UF,
	P,
	W,
	C,
	F,
	V,
	SP
};

typedef union _idval
{
	double floatval;
	int64_t intval;
	uint64_t uintval;
	char *strval;
} idval;

/* TODO: better rewrite with two uint64_t for all types */

typedef struct _lexed
{
	int lexeme;
	int type;
	int typesize;
	int fracsize;
	char *idname;
	idval val;
	char signedness;
	int ptrlvl;
	int line;
	int character;
} lexed;

typedef struct _parsed
{
	lexed *lexeme;
	int syntax;
	idval val;
	int type;
	int typesize;
	int fracsize;
	int ptrlvl;
	char signedness;
	char *idname;
	int line;
	int character;
	struct _parsed *parent;
	int numchildren;
	struct _parsed **children;
} parsed;

void help(const char *name);
void ver(void);
int lex(lexed *lexlist, char *input, size_t inputlen);
void lexprint(lexed *lexeme);
int yylex(void);
void parseprint(parsed *stmt, int tab);

extern parsed PARSETREE;
extern lexed *CURRLEX;

#endif /* _CBC_H_ */
