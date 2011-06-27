CXXFLAGS = -std=gnu++0x -DUNICODE -Wall
LDFLAGS = -mwindows -municode -static-libgcc

BINARIES = kiosh.exe

kiosh.exe_SOURCES = library_loader.cpp main.cpp

include build/win-multi.mak
