CC = gcc
GTKFLAGS = `pkg-config --libs --cflags gtk+-2.0`
LDFLAGS = -lm
SRCS = gtk_ui.c  hashmap.c  main.c  myThread.c  schedls.c  Structures.c
OBJS = $(SRCS:.c=.o)
MAIN = program

all:	$(MAIN)
	@echo Compilation of $(MAIN) done

$(MAIN): $(SRCS)
	$(CC) $(SRCS) $(GTKFLAGS) -o $@ $(LDFLAGS)

clean:
	$(RM) *~ *.o $(MAIN)
