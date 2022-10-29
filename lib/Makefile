GCC_FLAGS=-W -Wall -Werror -pedantic
CLANG_FORMAT_FLAGS=-i --style=LLVM

all: data_dictionary.a clean-data_dictionary

data_dictionary.a: data_dictionary.o
	ar -rc $@ $<

data_dictionary.o: data_dictionary.c
	gcc -c $< -o $@ $(GCC_FLAGS)

data_dictionary.c:
	python3 generate_data_dictionary.py
	clang-format $@ data_dictionary.h $(CLANG_FORMAT_FLAGS)

clean-data_dictionary:
	rm -f data_dictionary.[!ah]