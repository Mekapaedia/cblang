%start parser, stmts;
%token LLTYPE, LLID, LLINT,	LLFLOAT, LLCHAR, LLPERIOD, LLSMICLN, LLCLN, LLCOMMA, LLRBRAC, LLLBRAC, LLRPAR, LLLPAR, LLRSRQ, LLLSRQ, LLARROW, LLMINUS, LLPLUS, LLMOD, LLSTAR, LLDIV, LLEXCL, LLAMP, LLTILDE, LLOR, LLAND, LLXOR, LLPIPE, LLCARET, LLRSHFT, LLRROT, LLLSHFT, LLLROT, LLEQ, LLNE, LLGT, LLGE, LLLT, LLLE, LLIF, LLELIF, LLELSE, LLWHILE, LLRET, LLSTRUCT, LLUNION, LLASSIGN, LLDLRSGN, LLBREAK, LLCONTINUE, LLCASTS, LLCASTZ, LLCASTO;
%options "generate-lexer-wrapper generate-llmessage generate-symbol-table";
%top {
#include "cbc.h"



}


stmts : 
	stmt morestmts
	{puts("stmts");}
;

morestmts :
	stmts
	{puts("morestmts");}
	|
;

stmt : 
	innerstmt endstmt
	{puts("stmt");}
;

endstmt : 
	LLSMICLN
	{puts("endstmt");}
;

innerstmt : 
	decl
	{puts("innerstmt");}
	| expr
	{puts("innerstmt");}
	| LLIF cond block elseif endif
	{puts("innerstmt");}
	| LLWHILE cond optanchor block
	{puts("innerstmt");}
	| LLRET expr
	{puts("innerstmt");}
	| LLSTRUCT LLID block
	{puts("innerstmt");}
	| LLUNION LLID block
	{puts("innerstmt");}
	| LLBREAK anchor
	{puts("innerstmt");}
	| LLCONTINUE anchor
	{puts("innerstmt");}
;

optanchor : 
	LLCLN LLID
	{puts("opanchor");}
	|
;

anchor : 
	LLID
	{puts("anchor");}
	|
;

block : 
	LLLBRAC morestmts endblock
	{puts("block");}
;

endblock : 
	LLRBRAC
	{puts("endblock");}
;

decl : 
	LLTYPE moredecl anotherdecl
	{puts("decl");}
;

moredecl : 
	ptr declname funcdel enddecl
	{puts("moredecl");}
;

ptr : 
	LLSTAR ptr
	{puts("ptr");}
	|
;

funcdel : 
	nocallfunc LLLPAR args endargs fwddecl
	{puts("funcdel");}
	|
;

nocallfunc :
	LLDLRSGN
	{puts("nocallfunc");}
	|
;

args : 
	decl
	{puts("args");}
	|
;

fwddecl : 
	block
	{puts("fwddecl");}
	|
;

endargs : 
	LLRPAR
	{puts("endargs");}
;

declname : 
	LLID
	{puts("declname");}
;

enddecl : 
	arr assign
	{puts("enddecl");}
;

assign : 
	LLASSIGN expr
	{puts("assign");}
	|
;

arr :
	LLLSRQ LLINT morearr
	{puts("arr");}
	|
;

morearr : 
	LLCOMMA LLINT morearr
	{puts("morearr");}
	| LLRSRQ
	{puts("morearr");}
;

anotherdecl : 
	LLCOMMA decl
	{puts("anotherdecl");}
	|
;

cond : 
	LLLPAR expr endcond
	{puts("cond");}
;

endcond : 
	LLRPAR
	{puts("endcond");}
;

elseif : 
	LLELIF cond block elseif
	{puts("elseif");}
	|
;

endif : 
	LLELSE block
	{puts("endif");}
	|
;

expr : 
	term more
	{puts("expr");}
	| LLLPAR expr moreparens more
	{puts("expr");}
;

moreparens : 
	LLRPAR
	{puts("moreparens");}
;


more :
	binop expr
	{puts("more");}
	| 
	{puts("more");}
;

term :
	unop term
	{puts("term");}
	| name expr2
	{puts("term");}
;

name : 
	LLID
	{puts("name");}
	| LLINT
	{puts("name");}
	| LLFLOAT
	{puts("name");}
	| LLCHAR
	{puts("name");}
;

expr2 : 
	arrexpr expr2
	{puts("expr2");}
	| funcall expr2
	{puts("expr2");}
	| structcall expr2
	{puts("expr2");}
	|
;

funcall : 
	LLLPAR expr morecall
	{puts("funcall");}
;

morecall : 
	LLCOMMA expr morecall
	{puts("morecall");}
	| LLRPAR
	{puts("morecall");}
;

structcall :
	LLPERIOD LLID
	{puts("structcall");}
	| LLARROW LLID
	{puts("structcall");}
;

arrexpr :
	LLLSRQ expr endarr
	{puts("arrexpr");}
;

endarr :
	LLCOMMA expr endarr
	{puts("endarr");}
	| LLRSRQ
	{puts("endarr");}
;

unop : 
	LLAMP
	{puts("unop");}
	| LLSTAR
	{puts("unop");}
	| LLEXCL
	{puts("unop");}
	| LLTILDE
	{puts("unop");}
	| cast
	{puts("unop");}
;

cast : 
	casttype morecast
	{puts("cast");}
;

casttype :
	LLCASTS
	{puts("casttype");}
	| LLCASTZ
	{puts("casttype");}
	| LLCASTO
	{puts("casttype");}
;

morecast : 
	LLLPAR endcast
	{puts("morecast");}
;

endcast :
	LLTYPE ptr moreparens
	{puts("endcast");}
;

binop :
	LLMINUS
	{puts("minus");}
	| LLPLUS
	{puts("plus");}
	| LLDIV
	| LLSTAR
	| LLOR
	| LLAND
	| LLXOR
	| LLPIPE
	| LLAMP
	| LLCARET
	| LLRSHFT
	| LLLSHFT
	| LLRROT
	| LLLROT
	| LLEQ
	| LLGT
	| LLGE
	| LLLT
	| LLLE
	| LLNE
	| LLMOD
	| LLASSIGN
	{puts("assign");}
;
