#!/bin/sh
gcc -o analysis main.c -lm
./analysis > output.txt
