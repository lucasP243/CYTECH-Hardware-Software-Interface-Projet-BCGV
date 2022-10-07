WORKING_DIR=.
GCC_FLAGS=-W -Wall -Werror -pedantic
CLANG_FORMAT_FLAGS=-i --style=LLVM

.PHONY: bin/app # To recompile bin/app everytime

all: debug

# ----- MAIN APPLICATION TARGET -----

debug: bin/app
	bin/driver &
	bin/app

bin/app: src/app.c
	gcc -I $(WORKING_DIR) -o $@ $^ lib/*.a

# ----- DATA_DICTIONARY TARGET -----

build-data_dictionary: lib/data_dictionary.a clean-data_dictionary

lib/data_dictionary.a: lib/data_dictionary.o
	ar -rc $@ $<

lib/data_dictionary.o: lib/data_dictionary.c
	gcc -c $< -o $@ $(GCC_FLAGS)

lib/data_dictionary.c:
	python3 generate_data_dictionary.py
	clang-format $@ lib/data_dictionary.h $(CLANG_FORMAT_FLAGS)

clean-data_dictionary: lib/data_dictionary.a
	rm lib/data_dictionary.[!ah]
