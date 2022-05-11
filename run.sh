#!/bin/bash

g++ -Og -g -shared -fPIC -o libtest.so test.cpp
build/standalone/Smeagle -l libtest.so
