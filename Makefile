CC=gcc
CFLAGS=-Wall -Werror -g
BUILD=build
SRC=src
EXEC=lecture_ELF modification_ELF
DEPENDS=$(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o $(BUILD)/liberation.o

all : $(EXEC)

lecture_ELF: $(BUILD)/lecture_ELF.o $(DEPENDS)
	$(CC) $(CFLAGS) -o $@ $^

modification_ELF: $(BUILD)/modification_ELF.o $(DEPENDS) $(BUILD)/renum_section.o $(BUILD)/correctionSymboles.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $<
	mv `basename $@` $(BUILD)

clean:
	rm -f $(BUILD)/*.o $(EXEC) *~ $(SRC)/*~

