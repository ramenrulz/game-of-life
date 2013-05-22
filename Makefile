# Makefile

CC =gcc
DEBUG=-g
CFLAGS=-Wall $(DEBUG)
LIBS=-lcurses
OBJS=main.o
EXE=gol 

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE) $(LIBS)

main.o: main.c main.h
	$(CC) $(CFLAGS) -c  main.c

run: $(EXE) 
	./$(EXE) 

clean:
	rm *.o
	rm $(EXE)
