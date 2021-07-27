CC = g++
CFLAGS = -Wall -Werror
TARGET = ipk-sniffer

.PHONY: all clean pack

all: $(TARGET)

$(TARGET): $(TARGET).o
		$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o -lpcap

$(TARGET).o: $(TARGET).cpp
		$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
		rm -f $(TARGET)
		rm -f $(TARGET).o
		rm -f xfiala61.tar

pack: all
	tar -cvf xfiala61.tar Makefile ipk-sniffer.cpp README manual.pdf