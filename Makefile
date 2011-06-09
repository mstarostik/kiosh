# This makefile is taylored to cross-compiling from Linux
# If you have a mingw environment on Windows it might work, too
# If you use a different toolchain, feel free to contribute a
# more generic makefile

ABIS := 32 64
BUILDDIR := build

ifeq ($(ABI),64)
CHOST = x86_64-pc-mingw32-
endif
ifeq ($(ABI),32)
CHOST = i686-pc-mingw32-
endif

CXX = $(CHOST)g++
LD = $(CHOST)g++
DLLTOOL = $(CHOST)dlltool
O = $(BUILDDIR)/$(ABI)

CXXFLAGS = -std=gnu++0x -DUNICODE -Wall
LDFLAGS = -mwindows -municode -static-libgcc

OBJS = main.o library_loader.o

.PHONY: all clean

ifeq ($(ABI),)
all:
	@for i in $(ABIS); do $(MAKE) ABI=$$i || exit 1; done
else
all: $(BUILDDIR)/$(ABI)/kiosh.exe
-include $(OBJS:%.o=$(O)/%.d)
endif

clean:
	rm -rf $(BUILDDIR)O = $(BUILDDIR)/$(ABI)


$(O)/kiosh.exe: $(addprefix $(O)/,$(OBJS))
	$(LD) $(LDFLAGS) -o $@ $^

$(O)/%.o: %.cpp Makefile
	@mkdir -p $(O)
	$(CXX) -c -MMD $(CXXFLAGS) -o $@ $<
