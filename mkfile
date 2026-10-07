CC = bear --append -- clang
DEBUG_FLAGS = -g -O0
COMPILER_ERRORS = -Wall -Werror -Wextra -Wpedantic
# This library is built with C23
PRIVATE_CFLAGS = $COMPILER_ERRORS $DEBUG_FLAGS -std=c23

# The header should support C89
PUBLIC_CFLAGS = $COMPILER_ERRORS $DEBUG_FLAGS -std=c89
DEPFILES = `{/bin/sh -c 'find . -name "*.d"'}

main.exe: main.o mt_rand.o
  $CC $prereq -o $target

<|cat $DEPFILES /dev/null

main.o: main.c
  $CC $PUBLIC_CFLAGS -c $prereq -o $target

%.o: %.c
  $CC $PRIVATE_CFLAGS -c $prereq -o $target

clean:V:
  rm -rf *.o *.exe *.d
