CC=gcc
CFLAGS=-Wall -Werror

.PHONY : clean


myShell : csapp.c csapp.h fctutil.h myshell.h jobs.h eval.c myshell.c parseline.c jobs.c fctutil.c
	 $(CC) $(CFLAGS) -o $@ $^ -lpthread -O0

clean :
	rm -fv myShell *.o *~*
