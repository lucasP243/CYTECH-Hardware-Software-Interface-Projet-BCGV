WORKING_DIR=.
GCC_FLAGS=-W -Wall -Werror -pedantic
CLANG_FORMAT_FLAGS=-i --style=LLVM

.PHONY: bin/app # To recompile bin/app everytime

all: build-data_dictionary debug

# ----- MAIN APPLICATION TARGET -----

debug: bin/app
	bin/driver &
	bin/app

bin/app: src/app.c $(wildcard src/state_machines/*.c)
	gcc -I $(WORKING_DIR) -o $@ $^ lib/*.a

clean:
	rm -f lib/data_dictionary.[ah]
	rm -f bin/app

# ----- DATA_DICTIONARY TARGET -----

.PHONY: lib/data_dictionary.a lib/data_dictionary.c

build-data_dictionary: lib/data_dictionary.a clean-data_dictionary

lib/data_dictionary.a: lib/data_dictionary.o
	ar -rc $@ $<

lib/data_dictionary.o: lib/data_dictionary.c
	gcc -c $< -o $@ $(GCC_FLAGS)

lib/data_dictionary.c:
	rm -f lib/data_dictionary.[ah]
	python3 generate_data_dictionary.py
	clang-format $@ lib/data_dictionary.h $(CLANG_FORMAT_FLAGS)

clean-data_dictionary:
	rm -f lib/data_dictionary.[!ah]
