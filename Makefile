WORKING_DIR=.
GCC_FLAGS=-W -Wall -Werror -pedantic
CLANG_FORMAT_FLAGS=-i --style=LLVM

.PHONY: bin/app # To recompile bin/app everytime

all: build-data_dictionary bin/app
	bin/driver &
	bin/app

bin/app: src/app.c $(wildcard src/state_machines/*.c)
	gcc -I $(WORKING_DIR) -o $@ $^ lib/*.a

build-data_dictionary:
	(cd lib; make all)

clean:
	rm -f lib/data_dictionary.[ah]
	rm -f bin/app
