#!/bin/bash

if [ $# -eq 1 ]
then
	if [ $1 == -v ]
	then
		valgrind="valgrind -q "
	else
		echo "Argument non reconnu : $1"
		exit 42
	fi
else
	valgrind=
fi

make -s modification_ELF

cd examples/
for i in example*.c
do
	if [ ! -e $(basename "$i" .c).s ]
	then
		arm-eabi-gcc -mno-thumb-interwork -S $i
	fi
done

cd ..
make -s examples

mkdir -p test/
cd build/
test=../test

arm-eabi-ld --section-start .text=0x58 --section-start .data=0x1000 -o example3_ld example3.o >/dev/null
../modification_ELF example3.o example3_mod .text=0x58 .data=0x1000 >/dev/null
arm-eabi-ld --section-start .text=0x100 -o example4_ld example4.o >/dev/null
../modification_ELF example4.o example4_mod .text=0x100 >/dev/null
arm-eabi-ld --section-start .text=0x120 -o example5_ld example5.o >/dev/null
../modification_ELF example5.o example5_mod .text=0x120 >/dev/null
arm-eabi-ld --section-start .text=0x400 -o example6_ld example6.o >/dev/null
../modification_ELF example6.o example6_mod .text=0x400 >/dev/null
arm-eabi-ld --section-start .text=0x142 -o example7_ld example7.o >/dev/null
../modification_ELF example7.o example7_mod .text=0x142 >/dev/null
#arm-eabi-ld --section-start .text=0x150 -o example8_ld example8.o >/dev/null
#../modification_ELF example8.o example8_mod .text=0x150 >/dev/null

for i in example*_ld
do
	arm-eabi-strip $i
done

for i in example*_mod example*_ld
do
	hd $i >$test/hd_$i.out
done

#cd $test
#for i in hd_*_mod.out
#do
#	diff $i $(basename "$i" _mod.out)_ld.out
#done
