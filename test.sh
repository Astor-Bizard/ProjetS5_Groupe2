#!/bin/bash

make -s lecture_ELF

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
	
	for j in `seq 0 20`
	do
		arm-eabi-readelf --hex-dump=$j $i >>$test/readelf_$i.out 2>>$test/readelf_$i.out
		../../lecture_ELF -x $j $i >>$test/lectureELF_$i.out 2>>$test/lectureELF_$i.out
	done
	for j in .text .data .ARM.attributes .shstrtab .symtab truc
	do
		arm-eabi-readelf --hex-dump=$j $i >>$test/readelf_$i.out 2>>$test/readelf_$i.out
		../../lecture_ELF -x $j $i >>$test/lectureELF_$i.out 2>>$test/lectureELF_$i.out
	done

	arm-eabi-readelf -s $i >>$test/readelf_$i.out
	../../lecture_ELF -s $i >>$test/lectureELF_$i.out
	arm-eabi-readelf -r $i >>$test/readelf_$i.out
	../../lecture_ELF -r $i >>$test/lectureELF_$i.out

	diff $test/lectureELF_$i.out $test/readelf_$i.out
done
