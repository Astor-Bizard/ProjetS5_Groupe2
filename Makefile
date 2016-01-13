CC=gcc
CFLAGS=-Wall -Werror -g
BUILD=build
SRC=src
EXEC=lecture_ELF modification_ELF ARM_runner_exec
DEPENDS_ALL=$(BUILD)/lecture_headers.o $(BUILD)/lectureSH.o $(BUILD)/afficher_section.o $(BUILD)/lectureST.o $(BUILD)/affichage_relocation.o $(BUILD)/liberation.o
DEPENDS_MOD=$(BUILD)/renum_section.o $(BUILD)/correctionSymboles.o $(BUILD)/reimpl_R_ARM.o
DEPENDS_EXEC=$(BUILD)/lecture_headers.o (BUILD)/arm_simulator_interface.o (BUILD)/debug.o

all : $(EXEC)

lecture_ELF: $(BUILD)/lecture_ELF.o $(DEPENDS_ALL)
	$(CC) $(CFLAGS) -o $@ $^

modification_ELF: $(BUILD)/modification_ELF.o $(DEPENDS_ALL) $(DEPENDS_MOD)
	$(CC) $(CFLAGS) -o $@ $^

ARM_runner_exec: $(BUILD)/ARM_runner_exec.o $(DEPENDS_ALL)

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $<
	mv `basename $@` $(BUILD)

clean:
	rm -f $(BUILD)/*.o $(EXEC) *~ $(SRC)/*~

