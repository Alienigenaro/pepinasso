#!/bin/bash
cp -a arquivos/. .
make -f Makefile 

for i in {1..15}
do
        ./programaTrab2 < $i.in > $i.o
done

for i in {1..15}
do
        cmp $i.o $i.out
done
