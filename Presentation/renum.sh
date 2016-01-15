#!/bin/sh

echo "Fichier de base :\n\n\n"
./../lecture_ELF -e ../build/example3.o

echo "\n\n\n"
echo "Fichier executable : \n\n\n"

./../modification_ELF ../build/example3.o ../test/example3_ld .text=0x58 .data=0x1000



