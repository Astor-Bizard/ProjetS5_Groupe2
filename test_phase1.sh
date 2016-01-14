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

make -s lecture_ELF

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
cd build
test=../test

for i in example*.o
do
	arm-eabi-readelf -h $i >$test/readelf_$i.out
	$valgrind../lecture_ELF -h $i >$test/lectureELF_$i.out
	arm-eabi-readelf -S $i >>$test/readelf_$i.out
	$valgrind../lecture_ELF -S $i >>$test/lectureELF_$i.out
	
	for j in `seq 0 20`
	do
		arm-eabi-readelf --hex-dump=$j $i >>$test/readelf_$i.out 2>>$test/readelf_$i.out
		$valgrind../lecture_ELF -x $j $i >>$test/lectureELF_$i.out 2>>$test/lectureELF_$i.out
	done
	for j in .text .data .ARM.attributes .shstrtab .symtab truc
	do
		arm-eabi-readelf --hex-dump=$j $i >>$test/readelf_$i.out 2>>$test/readelf_$i.out
		$valgrind../lecture_ELF -x $j $i >>$test/lectureELF_$i.out 2>>$test/lectureELF_$i.out
	done

	arm-eabi-readelf -s $i >>$test/readelf_$i.out
	$valgrind../lecture_ELF -s $i >>$test/lectureELF_$i.out
	arm-eabi-readelf -r $i >>$test/readelf_$i.out
	$valgrind../lecture_ELF -r $i >>$test/lectureELF_$i.out

	diff $test/lectureELF_$i.out $test/readelf_$i.out
done
