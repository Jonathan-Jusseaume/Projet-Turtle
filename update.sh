#!/bin/sh

gcc -Wall -Wextra -std=c99 tortank.c -lm
(cd turtlepicross; java -jar turtlepicross.jar -b1 C:../a.out);