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

#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

#include "library_loader.hpp"

void runInstallUninstallStubs() {
    if (library_loader shell32 = {L"shell32.dll"}) {
	// Windows 7 x86_64 at least
	if (auto RunInstallUninstallStubs2 = shell32.symbol< void, int >(885)) RunInstallUninstallStubs2(0);
    }
    else if (library_loader shdocvw = {L"shdocvw.dlL"}) {
	// XP x86 at least
	if (auto RunInstallUninstallStubs = shdocvw.symbol< void >(125)) RunInstallUninstallStubs();
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
	MessageBox(0, L"Usage: kiosh <command to run>", L"kiosh", MB_ICONERROR);
	return EXIT_FAILURE;
    }

    runInstallUninstallStubs();

    _spawnvp(_P_NOWAIT, argv[1], (const char *const *)argv + 1);

    return EXIT_SUCCESS;
}
