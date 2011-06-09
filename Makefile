# This makefile is taylored to cross-compiling from Linux
# If you have a mingw environment on Windows it might work, too
# If you use a different toolchain, feel free to contribute a
# more generic makefile

CC = i686-pc-mingw32-gcc
LD = i686-pc-mingw32-gcc
DLLTOOL = i686-pc-mingw32-dlltool

CFLAGS = -DUNICODE
LDFLAGS = -mwindows -municode

.PHONY: all clean

all: kiosh.exe

clean:
	rm -f kiosh.exe *.o *.a

kiosh.exe: main.o libshdocvw.a
	$(LD) $(LDFLAGS) -o $@ $^

%.a: %.def
	$(DLLTOOL) -l $@ -d $<
