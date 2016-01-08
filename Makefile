CC=gcc
CFLAGS=-Wall -Werror
BUILD=build
SRC=src
EXEC=lecture_ELF modification_ELF

lecture_ELF: $(BUILD)/lecture_ELF.o $(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o
	$(CC) $(CFLAGS) -o $@ $^

modification_ELF: $(BUILD)/modification_ELF.o $(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o $(BUILD)/renum_section.o


$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $<
	mv `basename $@` $(BUILD)

$(BUILD)/lecture_ELF.o: $(SRC)/lecture_headers.h $(SRC)/lectureSH.h $(SRC)/afficher_section.h $(SRC)/affichage_relocation.h $(SRC)/lectureST.h
$(BUILD)/lectureSH.o: $(SRC)/lecture_headers.h $(SRC)/lectureSH.h
$(BUILD)/afficher_section.o: $(SRC)/lecture_headers.h $(SRC)/lectureSH.h $(SRC)/afficher_section.h
$(BUILD)/lectureST.o: $(SRC)/lecture_headers.h $(SRC)/lectureSH.h $(SRC)/lectureST.h
$(BUILD)/affichage_relocation.o: $(SRC)/lecture_headers.h $(SRC)/afficher_section.h $(SRC)/affichage_relocation.h $(SRC)/lectureST.h
$(BUILD)/renum_section.o: $(SRC)/renum_section.h $(SRC)/afficher_section.h $(SRC)/lectureST.h $(SRC)/affichage_relocation.h
$(BUILD)/liberation.o: $(SRC)/lectureST.h $(SRC)/lectureSH.h $(SRC)/affichage_relocation.h
clean:
	rm -f $(BUILD)/*.o $(EXEC) *~ $(SRC)/*~

