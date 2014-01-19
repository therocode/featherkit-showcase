#!/bin/sh
mv bin/rain bin/rain.bc
#../emscripten/emcc bin/rain.bc libs/* -o index.html -s FULL_ES2=1 -O2 --llvm-opts 3 --llvm-lto 3 -s OUTLINING_LIMIT=5000
../emscripten/emcc bin/rain.bc libs/* -o index.html --embed-file raindata/ -s FULL_ES2=1 -O2 --llvm-opts 3 --llvm-lto 3 -s TOTAL_MEMORY=33554432 -s OUTLINING_LIMIT=5000
