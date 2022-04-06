#
# Makefile that builds btest and other helper programs for the CS:APP data lab
# 
CC = gcc
CFLAGS = -O -Wall -pedantic

all: mm mm-mt

mm: mm.c mm.h 
	$(CC) $(CFLAGS) -o mm mm.c

mm-mt: mm-mt.c mm-mt.h
	$(CC) $(CFLAGS) -o mm-mt mm.c

clean:
	rm -f *.o mm
	rm -f *.o mm-mt
