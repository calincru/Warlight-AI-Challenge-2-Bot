#!/bin/bash

mkdir -p archive

cp -R ./include/* archive
cp -R ./src/*.cpp archive
cp -R ./src/*.h archive

tar czvf AproapeBine.tar.gz -C ./archive .

rm -rf archive &>/dev/null
