%start parser, stmts;
%token LLTYPE, LLID, LLINT,	LLFLOAT, LLCHAR, LLPERIOD, LLSMICLN, LLCLN, LLCOMMA, LLRBRAC, LLLBRAC, LLRPAR, LLLPAR, LLRSRQ, LLLSRQ, LLARROW, LLMINUS, LLPLUS, LLMOD, LLSTAR, LLDIV, LLEXCL, LLAMP, LLTILDE, LLOR, LLAND, LLXOR, LLPIPE, LLCARET, LLRSHFT, LLRROT, LLLSHFT, LLLROT, LLEQ, LLNE, LLGT, LLGE, LLLT, LLLE, LLIF, LLELIF, LLELSE, LLWHILE, LLRET, LLSTRUCT, LLUNION, LLASSIGN, LLDLRSGN, LLBREAK, LLCONTINUE, LLCASTS, LLCASTZ, LLCASTO;
%options "generate-lexer-wrapper generate-llmessage generate-symbol-table";
%top {
#include "cbc.h"

parsed *curr = &PARSETREE;

void pushp(const char* str);
void pushparse(void);
void popparse(void);

void pushp(const char* str) 
{
	puts(str);
	fflush(stdout);
}

void pushparse(void)
{
	parsed *new = malloc(sizeof(struct _parsed));
	if(new == NULL)
	{
		perror(MEMALLOCERR);
		return;
	}
	new->parent = curr;
	new->lexeme = CURRLEX;
	new->numchildren = 0;
	new->children = NULL;
	printf("%lu\n", sizeof(struct _parsed**) * (size_t) (curr->numchildren + 1));
	curr->children = realloc(curr->children, sizeof(struct _parsed**) * (size_t) (curr->numchildren + 1));
	if(curr->children == NULL)
	{
		perror(MEMALLOCERR);
		return;
	}
	curr->children[curr->numchildren] = new;
	curr->numchildren++;
	curr = new;
}

void popparse(void)
{
	curr = curr->parent;
}


}


stmts : 
	{pushp("stmts");}
	stmt morestmts
;

morestmts :
	{pushp("morestmts");}
	stmts
	| {pushp("morestmts-e");}
;

stmt : 
	{pushp("stmt");}
	innerstmt endstmt
;

endstmt : 
	{pushp("endstmt");}
	LLSMICLN
;

innerstmt : 
	{pushp("innerstmt"); pushparse();} decl {popparse();}
	| {pushp("innerstmt"); pushparse();} expr {popparse();}
	| {pushp("innerstmt"); pushparse();} LLIF cond block elseif endif {popparse();}
	| {pushp("innerstmt"); pushparse();} LLWHILE cond optanchor block {popparse();}
	| {pushp("innerstmt"); pushparse();} LLRET expr {popparse();}
	| {pushp("innerstmt"); pushparse();} LLSTRUCT LLID block {popparse();}
	| {pushp("innerstmt"); pushparse();} LLUNION LLID block {popparse();}
	| {pushp("innerstmt"); pushparse();} LLBREAK anchor {popparse();}
	| {pushp("innerstmt"); pushparse();} LLCONTINUE anchor {popparse();}
;

optanchor : 
	{pushp("opanchor"); pushparse();} LLCLN LLID {popparse();}
	| {pushp("opanchor-e");}
;

anchor :
	{pushp("anchor"); pushparse();} LLID {popparse();}
	| {pushp("anchor-e");}
;

block : 
	{pushp("block"); pushparse();} LLLBRAC morestmts endblock {popparse();}
;

endblock : 
	{pushp("endblock");} LLRBRAC
;

decl : 
	{pushp("decl");} LLTYPE moredecl anotherdecl
;

moredecl : 
	{pushp("moredecl"); pushparse();} ptr declname funcdel enddecl {popparse();}
;

ptr : 
	{pushp("ptr"); pushparse();} LLSTAR ptr {popparse();}
	| {pushp("ptr-e");}
;

funcdel : 
	{pushp("funcdel");} nocallfunc LLLPAR args endargs fwddecl
	| {pushp("funcdel-e");}
;

nocallfunc :
	{pushp("nocallfunc"); pushparse();} LLDLRSGN {popparse();}
	| {pushp("nocallfunc-e");}
;

args : 
	{pushp("args");}
	decl
	| {pushp("args-e");}
;

fwddecl :
	{pushp("fwddecl");} block
	| {pushp("fwddecl-e");}
;

endargs : 
	{pushp("endargs");}
	LLRPAR
;

declname : 
	{pushp("declname");}
	LLID
;

enddecl : 
	{pushp("enddecl"); pushparse();} arr assign {popparse();}
;

assign : 
	{pushp("assign"); pushparse();} LLASSIGN expr {popparse();}
	| {pushp("assign-e");}
;

arr :
	{pushp("arr");}
	LLLSRQ LLINT morearr
	| {pushp("arr-e");}
;

morearr : 
	{pushp("morearr");}
	LLCOMMA LLINT morearr
	| {pushp("morearr-end");} LLRSRQ
;

anotherdecl : 
	{pushp("anotherdecl");}
	LLCOMMA decl
	| {pushp("anotherdecl-e");}
;

cond : 
	{pushp("cond");}
	LLLPAR expr endcond
;

endcond : 
	{pushp("endcond");}
	LLRPAR
;

elseif : 
	{pushp("elseif");}
	LLELIF cond block elseif
	| {pushp("elseif-e");}
;

endif : 
	{pushp("endif");}
	LLELSE block
	| {pushp("endif-e");}
;

expr : 
	{pushp("expr");pushparse();} term more {popparse();}
	| {pushp("expr"); pushparse();} LLLPAR expr moreparens more {popparse();}
;

moreparens : 
	{pushp("moreparens");}
	LLRPAR
;


more :
	{pushp("more");}
	binop expr
	| {pushp("more-e");}
;

term :
	{pushp("term");}
	unop term
	| {pushp("term");} name expr2
;

name : 
	{pushp("name");}
	LLID
	| {pushp("name");} LLINT
	| {pushp("name");} LLFLOAT
	| {pushp("name");} LLCHAR
;

expr2 : 
	{pushp("expr2");}
	arrexpr expr2
	| {pushp("expr2");} funcall expr2
	| {pushp("expr2");} structcall expr2
	| {pushp("expr2-e");}
;

funcall : 
	{pushp("funcall");}
	LLLPAR expr morecall
;

morecall : 
	LLCOMMA expr morecall
	{pushp("morecall");}
	| LLRPAR
	{pushp("morecall");}
;

structcall :
	{pushp("structcall");}
	LLPERIOD LLID
	| {pushp("structcall");} LLARROW LLID
;

arrexpr :
	{pushp("arrexpr");}
	LLLSRQ expr endarr
;

endarr :
	{pushp("endarr");}
	LLCOMMA expr endarr
	| {pushp("endarr-end");} LLRSRQ
;

unop : 
	{pushp("unop");}
	LLAMP
	| {pushp("unop");} LLSTAR
	| {pushp("unop");} LLEXCL
	| {pushp("unop");} LLTILDE
	| {pushp("unop");} cast
;

cast : 
	{pushp("cast");}
	casttype morecast
;

casttype :
	{pushp("casttype");}
	LLCASTS
	| {pushp("casttype");} LLCASTZ
	| {pushp("casttype");} LLCASTO
;

morecast : 
	{pushp("morecast");}
	LLLPAR endcast
;

endcast :
	{pushp("endcast");}
	LLTYPE ptr moreparens
;

binop :
	{pushp("minus");}
	LLMINUS
	| {pushp("plus");} LLPLUS
	| {pushp("div");} LLDIV
	| {pushp("star");} LLSTAR
	| {pushp("or");} LLOR
	| {pushp("and");} LLAND
	| {pushp("xor");} LLXOR
	| {pushp("pipe");} LLPIPE
	| {pushp("amp");} LLAMP
	| {pushp("caret");} LLCARET
	| {pushp("rshift");} LLRSHFT
	| {pushp("lshift");} LLLSHFT
	| {pushp("rrotate");} LLRROT
	| {pushp("lrotate");} LLLROT
	| {pushp("eq");} LLEQ
	| {pushp("gt");} LLGT
	| {pushp("ge");} LLGE
	| {pushp("lt");} LLLT
	| {pushp("le");} LLLE
	| {pushp("ne");} LLNE
	| {pushp("mod");} LLMOD
	| {pushp("assign");} LLASSIGN
;
