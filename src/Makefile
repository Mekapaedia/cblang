SRC=cbc.c lex.c
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
	rm -rf $(EXE) ../$(EXE) *.dyn *.dpi *.lock *.stackdump *.db lex.c

phony: 
	true
