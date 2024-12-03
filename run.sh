#!/usr/bin/sh

gcc vector.c -o a.o && { ./a.o; rm a.o; }
