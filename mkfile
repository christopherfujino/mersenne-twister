PROJECT = mtrand
CC = bear --append -- clang
AR = llvm-ar
COMPILER_ERRORS = -Wall -Werror -Wextra -Wpedantic
# This library is built with C23
PRIVATE_CFLAGS = $COMPILER_ERRORS $DEBUG_FLAGS -std=c23

# The header should support C89
PUBLIC_CFLAGS = $COMPILER_ERRORS $DEBUG_FLAGS -std=c89
DEPFILES = `{/bin/sh -c 'find . -name "*.d"'}
MKSHELL = bash

lib$PROJECT.a: mt_rand.o check
	ARGS=""
	for arg in $prereq; do
		if [[ $arg != 'check' ]]; then
			ARGS="$arg $ARGS"
		fi
	done
	$AR rcs $target $ARGS

main.exe: main.o lib$PROJECT.a
	$CC $prereq -o $target

<|cat $DEPFILES /dev/null

main.o: main.c
	$CC $PUBLIC_CFLAGS -c $prereq -o $target

%.o: %.c
	$CC $PRIVATE_CFLAGS -c $prereq -o $target

check:V:
	set -e
	if [ -z "$DEBUG_FLAGS" ]; then
		echo "Please invoke via either ./build_debug.sh or ./build_release.sh"
		false
	fi

clean:V:
	rm -rf *.exe *.d *.o *.a compile_commands.json

# vim: ft=make
