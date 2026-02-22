#!/bin/bash

# Trap SIGINT (Ctrl+C) and exit the script
trap "exit" INT

b=./build/build/turtle_linux

b examples/hello.scm -a -d 100
b examples/polygons.scm -a -s -d 50
b examples/higher.js -a -s -d 50
b examples/recovery2.scm -a -d 10 -s
b examples/paradox.scm -a -s -d 10
b examples/boxy2.js -a -s -d 10
b examples/nexus.js -a -s -d 10
b examples/lux.js -a -s -d 19
