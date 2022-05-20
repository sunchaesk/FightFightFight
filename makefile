
LIBS = -lncurses

c:main.c
	gcc -Wall -g $(LIBS) main.c -o c

force:
	gcc -g $(LIBS) main.c -o c

clean:
	rm c
