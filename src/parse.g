%start parser, stmts;
%token LLTYPE, LLID, LLINT,	LLFLOAT, LLCHAR, LLPERIOD, LLSMICLN, LLCLN, LLCOMMA, LLRBRAC, LLLBRAC, LLRPAR, LLLPAR, LLRSRQ, LLLSRQ, LLARROW, LLMINUS, LLPLUS, LLMOD, LLSTAR, LLDIV, LLEXCL, LLAMP, LLTILDE, LLOR, LLAND, LLXOR, LLPIPE, LLCARET, LLRSHFT, LLRROT, LLLSHFT, LLLROT, LLEQ, LLNE, LLGT, LLGE, LLLT, LLLE, LLIF, LLELIF, LLELSE, LLWHILE, LLRET, LLSTRUCT, LLUNION, LLASSIGN, LLDLRSGN, LLBREAK, LLCONTINUE, LLCASTS, LLCASTZ, LLCASTO;
%options "generate-lexer-wrapper generate-llmessage generate-symbol-table";
%top {
#include "cbc.h"

}


stmts : 
	stmt morestmts
;

morestmts :
	stmts
	|
;

stmt : 
	innerstmt endstmt
;

endstmt : 
	LLSMICLN
;

innerstmt : 
	decl
	| expr
	| LLIF cond block elseif endif
	| LLWHILE cond optanchor block
	| LLRET expr
	| LLSTRUCT LLID block
	| LLUNION LLID block
	| LLBREAK anchor
	| LLCONTINUE anchor
;

optanchor : 
	LLCLN LLID
	|
;

anchor : 
	LLID
	|
;

block : 
	LLLBRAC morestmts endblock
;

endblock : 
	LLRBRAC
;

decl : 
	LLTYPE moredecl anotherdecl
;

moredecl : 
	ptr declname funcdel enddecl
;

ptr : 
	LLSTAR ptr
	|
;

funcdel : 
	nocallfunc LLLPAR args endargs fwddecl
	|
;

nocallfunc :
	LLDLRSGN
	|
;

args : 
	decl
	|
;

fwddecl : 
	block
	|
;

endargs : 
	LLRPAR
;

declname : 
	LLID
;

enddecl : 
	arr assign
	|
;

assign : 
	LLASSIGN expr
;

arr :
	LLLSRQ LLINT morearr
;

morearr : 
	LLCOMMA LLINT morearr
	| LLRSRQ
;

anotherdecl : 
	LLCOMMA decl
	|
;

cond : 
	LLLPAR expr endcond
;

endcond : LLRPAR
;

elseif : 
	LLELIF cond block elseif
	|
;

endif : 
	LLELSE block
	|
;

expr : 
	unop basename
	| basename morexpr
	| LLLPAR expr moreparens
;

moreparens : 
	LLRPAR
;

basename : 
	name expr2
;

morexpr :
	binop expr
	|
;

name : 
	LLID
	| LLINT
	| LLFLOAT
	| LLCHAR
;

expr2 : 
	arrexpr expr2
	| funcall expr2
	| structcall expr2
	|
;

funcall : 
	LLLPAR expr morecall
;

morecall : 
	LLCOMMA expr morecall
	| LLRPAR
;

structcall :
	LLPERIOD LLID
	| LLARROW LLID
;

arrexpr :
	LLLSRQ expr endarr
;

endarr :
	LLCOMMA expr endarr
	| LLRSRQ
;

unop : 
	LLAMP
	| LLSTAR
	| LLEXCL
	| LLTILDE
	| cast
;

cast : 
	casttype morecast
;

casttype :
	LLCASTS
	| LLCASTZ
	| LLCASTO
;

morecast : 
	LLLPAR endcast
;

endcast :
	LLTYPE ptr moreparens
;

binop :
	LLMINUS
	| LLPLUS
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
;
