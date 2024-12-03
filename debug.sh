#!/usr/bin/sh

gcc vector.c -o a.o -g && { gdb a.o; rm a.o; }
