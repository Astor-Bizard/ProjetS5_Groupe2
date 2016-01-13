CC=gcc
CFLAGS=-Wall -Werror -g
BUILD=build
SRC=src
ELF=elf_linker-1.0
EXEC=lecture_ELF modification_ELF
DEPENDS_ALL=$(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o $(BUILD)/liberation.o
DEPENDS_MOD=$(BUILD)/renum_section.o $(BUILD)/correctionSymboles.o $(BUILD)/reimpl_R_ARM.o


all: $(EXEC)

lecture_ELF: $(BUILD)/lecture_ELF.o $(DEPENDS_ALL)
	$(CC) $(CFLAGS) -o $@ $^

modification_ELF: $(BUILD)/modification_ELF.o $(DEPENDS_ALL) $(DEPENDS_MOD)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $<
	mv `basename $@` $(BUILD)

clean:
	rm -f $(BUILD)/*.o $(EXEC) *~ $(SRC)/*~
