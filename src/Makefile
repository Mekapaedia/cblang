SRC=cbc.c lex.c parse.c
EXE=cbc
EXTRACFLAGS=
EXTRALDFLAGS=
export SRC
export EXE
export EXTRACFLAGS
export EXTRALDFLAGS

all: debug

fast: phony
	$(MAKE) -f Makefile.fast.gcc

debug: phony
	$(MAKE) -f Makefile.dev.gcc

clean:
	rm -rf *.gcno *.gcda *.gcov $(SRC:.c=.o)
	rm -rf $(EXE) ../$(EXE) *.dyn *.dpi *.lock *.stackdump *.db lex.c parse.c parse.h gmon.out

phony: 
	true
