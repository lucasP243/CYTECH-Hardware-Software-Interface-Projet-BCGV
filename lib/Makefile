GCC_FLAGS=-W -Wall -Werror -pedantic
CLANG_FORMAT_FLAGS=-i --style=LLVM

# $(call compile_static_library, lib_name)
# Compiles "lib_name.c" into object file, then pack it into archive file "lib_name.a" (static_library)
define compile_static_library
	gcc -c $(1).c -o $(1).o $(GCC_FLAGS)
	ar -rc $(1).a $(1).o
	rm -f $(1).[!ah]
endef

all: data_dictionary.a crc8.a

data_dictionary.a: data_dictionary.c data_dictionary.h
	clang-format $^ $(CLANG_FORMAT_FLAGS)
	$(call compile_static_library, data_dictionary)

data_dictionary.c:
	(cd python; make)

checksum.h:
	wget -q --output-document=$@ https://raw.githubusercontent.com/lammertb/libcrc/master/include/checksum.h

crc8.c:
	wget -q --output-document=$@ https://raw.githubusercontent.com/lammertb/libcrc/master/src/crc8.c

crc8.a: checksum.h crc8.c
	$(call compile_static_library, crc8)

clean:
	rm -f data_dictionary.*
	rm -f crc8.*
	rm -f checksum.h