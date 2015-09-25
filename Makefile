# Compiler:
CC = g++

# Flags:
# -Wall -pedantic
CFLAGS = -O3

# More flags:
FRAMEWORKS = -lsgct -framework Opengl -framework Cocoa -framework IOKit -framework CoreVideo -stdlib=libstdc++



MKDIR = mkdir -p bin
LIBFOLD = -L"/usr/local/lib"
INCFOLD = -I"/usr/local/include"


# Files:
FILES = $(wildcard src/*.cpp)

# Binary folder:
BINFOLD = bin/

# Binary name:
BINNAME = clothsim

FOO = this is not windows



all: compile
.PHONY: all

compile: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(BINFOLD)$(BINNAME) $(LIBFOLD) $(INCFOLD) $(FRAMEWORKS)
.PHONY: compile

run:
	./$(BINFOLD)$(BINNAME) -config "configs/single.xml"
.PHONY: run

report:
	cd docs && latex report.tex && bibtex refs && latex report.tex && latex report.tex && cd ..
.PHONY: run

talk:
	$(FOO)
.PHONY: run

clean:
	rm -f $(BINFOLD)*
.PHONY: clean
