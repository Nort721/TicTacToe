#!/bin/bash
cd "/Users/nort/files/workspace/eclipse-workspace/TicTacToe/src/lib"
gcc -shared -fPIC engine.c uiapi.c -I./headers -o /Users/nort/Desktop/engine.so