#!/bin/bash

make -s

mkdir -p test/

cd elf_linker-1.0/Examples_loader/
test=../../test
arm-eabi-as -o example1_as.o example1.s
arm-eabi-as -o example2_as.o example2.s
arm-eabi-as -o example3_as.o example3.s
arm-eabi-as -o example4_as.o example4.s
for i in example*.o
do
	arm-eabi-readelf -h $i >$test/readelf_$i.out
	../../lecture_ELF -h $i >$test/lectureELF_$i.out
	arm-eabi-readelf -S $i >>$test/readelf_$i.out
	../../lecture_ELF -S $i >>$test/lectureELF_$i.out
	arm-eabi-readelf --hex-dump=1 $i >>$test/readelf_$i.out
	../../lecture_ELF -x 1 $i >>$test/lectureELF_$i.out
	arm-eabi-readelf -s $i >>$test/readelf_$i.out
	../../lecture_ELF -s $i >>$test/lectureELF_$i.out
	arm-eabi-readelf -r $i >>$test/readelf_$i.out
	../../lecture_ELF -r $i >>$test/lectureELF_$i.out

	diff $test/lectureELF_$i.out $test/readelf_$i.out
done
