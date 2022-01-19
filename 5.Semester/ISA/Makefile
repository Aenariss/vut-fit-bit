CC = g++
CFLAGS = -Wall -Werror -pedantic
LFLAGS= -lpcap
TARGET = client
LIBS = base64.o
OBJ = $(TARGET).o $(LIBS)


.PHONY: all clean pack

all: $(TARGET) clean

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
		rm -f $(OBJ)

pack: all
	tar -cvf xfiala61.tar Makefile client.cpp manual.pdf isa.pcap isa.lua base64.h base64.cpp