CFLAGS=-g -Wall
LDFLAGS=-lpthread
TS=_ts
# -g includes debugging symbols
# -Wall report all warnings; treat as errors

all: rbtest threadtest keytest

rbtest: rbtest.o ringbuffer$(TS).o
	gcc -o rbtest rbtest.o ringbuffer$(TS).o $(LDFLAGS)

threadtest: threadtest.o ringbuffer$(TS).o
	gcc -o threadtest threadtest.o ringbuffer$(TS).o $(LDFLAGS)

keytest: keytest.o ringbuffer$(TS).o
	gcc -o keytest keytest.o ringbuffer$(TS).o $(LDFLAGS)

# targets that don't depend on real file dependencies
.PHONY: clean test

# delete intermediate files
clean:
	$(RM) threadtest rbtest keytest *.o *~
