WORKING_DIR=./
GCC_FLAGS=-W -Wall -Werror -pedantic

.PHONY: bin/app # To recompile bin/app everytime

all: build-libraries bin/app

bin/app: src/app.c $(wildcard src/state_machines/*.c)
	gcc -I $(WORKING_DIR) -o $@ $^ lib/*.a

build-libraries:
	(cd lib; make all)

clean:
	(cd lib; make clean)
	rm -f bin/app
