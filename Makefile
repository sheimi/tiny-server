CC=gcc
SRC=src
INC=include
CFLAGS=-I$(INC) -Wall -c
OBJS=server.o dispatcher.o child_pool.o wrappers.o
TARGET=server
ifneq ($(DEBUG),)
	CFLAGS += -g -DDEBUG 
else
	CFLAGS += -O2
endif

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) $<

dispatcher.o: $(SRC)/dispatcher.c
	$(CC) $(CFLAGS) $<

child_pool.o: $(SRC)/child_pool.c
	$(CC) $(CFLAGS) $<

wrappers.o: $(SRC)/wrappers.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
