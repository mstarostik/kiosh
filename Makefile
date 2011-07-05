CXXFLAGS = -std=gnu++0x -DUNICODE -Wall
LDFLAGS = -mwindows -municode -static-libgcc

BINARIES = kiosh.exe

kiosh.exe_SOURCES = event.cpp library_loader.cpp main.cpp os_version.cpp

include build/win-multi.mak
