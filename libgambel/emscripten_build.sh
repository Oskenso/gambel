#!/bin/sh
emcc libgambel.c cpu.c -fPIC -shared -Wall -O2 -s EXPORTED_FUNCTIONS="['_GB_Create', '_GB_Destroy']" -o libgambel.js
