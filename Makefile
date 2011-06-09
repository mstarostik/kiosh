# This makefile is taylored to cross-compiling from Linux
# If you have a mingw environment on Windows it might work, too
# If you use a different toolchain, feel free to contribute a
# more generic makefile

ABIS := 32 64
BUILDDIR := build

CC_32 := i686-pc-mingw32-gcc
LD_32 := i686-pc-mingw32-gcc
DLLTOOL_32 = i686-pc-mingw32-dlltool

CC_64 := x86_64-pc-mingw32-gcc
LD_64 := x86_64-pc-mingw32-gcc
DLLTOOL_64 := x86_64-pc-mingw32-dlltool

CC = $(CC_$(ABI))
LD = $(LD_$(ABI))
DLLTOOL = $(DLLTOOL_$(ABI))
O = $(BUILDDIR)/$(ABI)

CFLAGS = -DUNICODE -Wall
LDFLAGS = -mwindows -municode

OBJS = main.o libshdocvw.a

.PHONY: all clean

ifeq ($(ABI),)
all:
	@for i in $(ABIS); do $(MAKE) ABI=$$i || exit 1; done
else
all: $(BUILDDIR)/$(ABI)/kiosh.exe
endif

clean:
	rm -rf $(BUILDDIR)O = $(BUILDDIR)/$(ABI)


$(O)/kiosh.exe: $(addprefix $(O)/,$(OBJS))
	$(LD) $(LDFLAGS) -o $@ $^

$(O)/%.o: %.c
	@mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(O)/%.a: %.def
	@mkdir -p $(O)
	$(DLLTOOL) -l $@ -d $<
