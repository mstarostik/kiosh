/*
    kiosh - Session initialization helper for Windows based systems
    that do not use explorer.exe for a shell

    Copyright © 2011 Malte Starostik

    kiosh is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    kiosh is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with kiosh.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <windows.h>

#include "library_loader.hpp"

struct entry_point {
    std::wstring library;
    unsigned int ordinal;
};

std::vector< entry_point > candidates = {
    {L"shell32.dll", 885}, // since Windows 7
    {L"shdocvw.dll", 130}  // at least since Windows 2000
};

void runInstallUninstallStubs() {
    for (auto it = candidates.begin(); it != candidates.end(); ++it)
	if (library_loader loader = {it->library})
	    if (auto RunInstallUninstallStubs2 = loader.symbol< void WINAPI (const wchar_t*) >(it->ordinal)) {
		RunInstallUninstallStubs2(static_cast< const wchar_t* >(0)); // Yuck...
		return;
	    }
}


int main(int argc, char *argv[]) {
#ifndef __WIN64__
    if (library_loader loader = {L"kernel32.dll"})
	if (auto IsWow64Process = loader.symbol< BOOL WINAPI (HANDLE, BOOL*) >("IsWow64Process")) {
	    BOOL runningOn64 = false;
	    if (IsWow64Process(GetCurrentProcess(), &runningOn64) && runningOn64) {
		MessageBox(0, L"The 32-bit version of kiosh will not work on a 64-bit OS.  Please use the 64-bit version instead.", L"kiosh", MB_ICONERROR);
		return EXIT_FAILURE;
	    }
	}
#endif

    if (argc < 2) {
	MessageBox(0, L"Usage: kiosh <command to run>", L"kiosh", MB_ICONERROR);
	return EXIT_FAILURE;
    }

    runInstallUninstallStubs();

    _spawnvp(_P_NOWAIT, argv[1], (const char *const *)argv + 1);

    return EXIT_SUCCESS;
}
