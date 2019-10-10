
SHELL = /bin/sh

# ----------------------------------------------------------------------
# COMPILE OPTIONS
# ----------------------------------------------------------------------
MAIN_NAME = CannonBall
# Compiler to be used.
CC = gcc

# External headers for ptask.
INCLUDE = -I./include

# Options to the compiler.
CFLAGS = -Wall -lrt -lm
ALL_FLAGS = $(INCLUDE) $(CFLAGS)

# Libraries.
LIB_PTASK = -L./lib -lptask
LIB_ALLEGRO = -lpthread `allegro-config --libs`

LIBS = $(LIB_PTASK) $(LIB_ALLEGRO)

# ----------------------------------------------------------------------
# FILES AND DIRECTORIES
# ----------------------------------------------------------------------

# Directory with output compiled files.
OUT_BUILD = ./build

# Source files.
SRC = ./src

# Target filename.
MAIN = main
SOURCE = manager graphic shot target

# Files to compile.
BASE_FILES = $(MAIN) $(SOURCE)
SOURCE_FILES = $(addsuffix .c, $(addprefix $(SRC)/, $(BASE_FILES)))
OUT_FILES = $(addsuffix .o, $(addprefix $(OUT_BUILD)/, $(BASE_FILES)))

# ----------------------------------------------------------------------
# TARGETS
# ----------------------------------------------------------------------

# all call clean and build
all: clean build

# -------------------------
# BUILD
# -------------------------

# Build.
build: compile link 

# Compile all specified source files.
compile: $(SOURCE_FILES)
	$(foreach f, $^, \
		$(CC) -g -c $f -o $(OUT_BUILD)/$(basename $(notdir $f)).o $(ALL_FLAGS);)

# Link all builded source files and create executable.
link: $(OUT_FILES)
	$(CC) -o $(OUT_BUILD)/$(MAIN) $(OUT_FILES) $(LIBS) $(ALL_FLAGS)

# ----------------------------------
# CLEAN
# ----------------------------------

# Command to clean: make clean
clean:
	rm -f $(OUT_BUILD)/*

#	# ---------------------
# SECTION: RUN
#	# ---------------------

check-env:
ifeq ($(DISPLAY),)
	$(error Display mode NOT supported.)
else
	$(info Display mode supported.)
endif

# Clean, build and run as superuser (in order to use ptask).
run: check-env all
	$(info Executing $(MAIN_NAME))
	sudo $(OUT_BUILD)/$(MAIN)
