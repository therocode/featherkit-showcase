#!/bin/sh 
cp ../featherkitemscripten/lib/libfeatherkit-*.so libs/
mv bin/rain bin/rain.bc
../emscripten/emcc bin/rain.bc libs/* -o index.html --embed-file raindata/ -s FULL_ES2=1
#../emscripten/emcc bin/rain.bc libs/* -o index.html --embed-file raindata/ -s FULL_ES2=1 -O2 --llvm-opts 3 --llvm-lto 3
