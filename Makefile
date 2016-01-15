CC=gcc
CFLAGS=-Wall -Werror -g
BUILD=build
SRC=src
ELF=elf_linker-1.0
EXEC=lecture_ELF modification_ELF
DEPENDS_ALL=$(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o $(BUILD)/liberation.o
DEPENDS_MOD=$(BUILD)/renum_section.o $(BUILD)/correctionSymboles.o $(BUILD)/reimpl_R_ARM.o $(BUILD)/ecritureSH.o 
EX=$(BUILD)/example
EXAMPLES=$(EX)1.o $(EX)2.o $(EX)3.o $(EX)4.o $(EX)5.o $(EX)6.o $(EX)7.o $(EX)8.o

all: $(EXEC)

lecture_ELF: $(BUILD)/lecture_ELF.o $(DEPENDS_ALL)
	$(CC) $(CFLAGS) -o $@ $^

modification_ELF: $(BUILD)/modification_ELF.o $(DEPENDS_ALL) $(DEPENDS_MOD)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $<
	mv `basename $@` $(BUILD)

examples: $(EXAMPLES)

$(EX)%.o: examples/example%.s
	arm-eabi-as -o $@ $<

clean:
	rm -f $(BUILD)/* $(EXEC) *~ $(SRC)/*~

clean_all: clean
	rm -rf test/
	rm -df $(BUILD)
