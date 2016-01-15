#!/bin/bash


./../modification_ELF ../build/example3.o example3_out.o .text=0x58 .data=0x1000 

hd example3_out.o