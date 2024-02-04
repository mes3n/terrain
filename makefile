# Variables to control Makefile operation

CC = gcc
MYDIR = .
SRC_DIR = $(MYDIR)/src
DEP_DIR = $(MYDIR)/deps
OBJ_DIR = $(MYDIR)/obj
CFLAGS = -Wall -Wextra -pedantic -g -Ideps
LDFLAGS = -Wall -Wextra -pedantic -g -lm -lglfw -lGL

# create list of object files from source files but replace ".cpp" and "src"
OBJ_SUBDIR = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(shell find $(SRC_DIR)/* -type d))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(shell find $(SRC_DIR)/* -name *.c))

DEP_OBJ_FILES = $(patsubst %.c, %.o, $(shell find $(DEP_DIR)/* -name *.c))

# For windoes or other systems with lack of find
# OBJ_SUBDIR = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(wildcard $(SRC_DIR)/*/ $(SRC_DIR)/*/*/))# two levels of subdirectories
# OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*/*.c))

main: $(OBJ_FILES) $(DEP_OBJ_FILES)
	$(CC) -o bin/$@ $^ $(LDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

all: clean main

run: main
	@echo ""
	exec $(MYDIR)/bin/main

deps: cldeps $(DEP_OBJ_FILES)

cldeps:
	rm -f $(DEP_OBJ_FILES)

$(DEP_DIR)/%.o: $(DEP_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)


header: HEADER_NAME = $(subst .,_,$(shell echo $(lastword $(subst /, ,$(FILE))) | tr '[:lower:]' '[:upper:]'))
		SOURCE_FILE = $(subst .h,.c,$(FILE))

header: 
ifeq ("$(wildcard $(FILE))", "")
	@touch $(FILE) 
	@touch $(SOURCE_FILE)
	@echo '#ifndef $(HEADER_NAME)' >> $(FILE)
	@echo '#define $(HEADER_NAME)' >> $(FILE)
	@echo '' >> $(FILE)
	@echo '' >> $(FILE)
	@echo '' >> $(FILE)
	@echo '#endif  // $(HEADER_NAME)' >> $(FILE)

	@echo "Created c++ header file ($(FILE)) with header guard ($(HEADER_NAME)) and source file ($(SOURCE_FILE))."
	git add $(FILE) $(SOURCE_FILE)

else
	@echo "Header file already exists."

endif


.PHONY: clean
clean:
	rm -rf bin obj
	mkdir bin obj $(OBJ_SUBDIR)
