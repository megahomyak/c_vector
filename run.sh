#!/usr/bin/sh

gcc usage.c -o a.o && { ./a.o; rm a.o; }
