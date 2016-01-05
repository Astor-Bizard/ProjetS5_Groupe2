CC=gcc
CFLAGS=-Wall -Werror

lecture_ELF: lecture_ELF.o lecture_headers.o lectureSH.o afficher_section.o lectureST.o affichage_relocation.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

lecture_ELF.o: lecture_headers.h lectureSH.h afficher_section.h lectureST.h affichage_relocation.h
lectureSH.o: lecture_headers.h lectureSH.h
afficher_section.o: lecture_headers.h lectureSH.h afficher_section.h
lectureST.o: lecture_headers.h lectureSH.h lectureST.h
affichage_relocation.o:	lecture_headers.h afficher_section.h affichage_relocation.h