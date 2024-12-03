#!/usr/bin/sh

gcc usage.c -o a.o -g && { gdb a.o; rm a.o; }
