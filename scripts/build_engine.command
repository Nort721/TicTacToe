#!/bin/bash
gcc -shared -fPIC ./src/lib/engine.c ./src/lib/uiapi.c -I./headers -o engine.so