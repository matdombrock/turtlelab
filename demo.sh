#!/bin/bash

# Trap SIGINT (Ctrl+C) and exit the script
trap "exit" INT

./build/turtle examples/hello.scm -a -d 100
./build/turtle examples/polygons.scm -a -s -d 50
./build/turtle examples/higher.js -a -s -d 50
./build/turtle examples/recovery2.scm -a -d 10 -s
./build/turtle examples/paradox.scm -a -s -d 10
./build/turtle examples/boxy2.js -a -s -d 10
./build/turtle examples/nexus.js -a -s -d 10
./build/turtle examples/lux.js -a -s -d 19