EXE=cbc
SRCDIR=src

all: debug

fast: phony
	$(MAKE) -C $(SRCDIR) fast
	cp $(SRCDIR)/$(EXE) .

debug: phony
	$(MAKE) -C src
	cp $(SRCDIR)/$(EXE) .

clean:
	$(MAKE) -C $(SRCDIR) clean
	rm -rf $(EXE) gmon.out

phony: 
	true
