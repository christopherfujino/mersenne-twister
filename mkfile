CC = bear --append -- clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c23

main.exe: main.o mt_rand.o
  $CC $prereq -o $target

%.o: %.c
  $CC $CFLAGS -c $prereq -o $target

clean:V:
  rm -rf *.o *.exe *.d
