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

#include "event.hpp"
#include "library_loader.hpp"
#include "os_version.hpp"

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

    event(os_version().major() >= 6 ? L"ShellDesktopSwitchEvent" : L"msgina: ShellReadyEvent").set();

    // FIXME: As Windows lacks anything that even remotely resembles sane quote handling wrt command line arguments,
    //        we don't pretend to really get it right.  We just remove what looks like being the first word (i.e. argv[0])
    //        and pass the rest untouched.
    std::wstring cmd = {GetCommandLine()};
    if (cmd.empty()) {
	MessageBox(0, L"Internal error", L"kiosh", MB_ICONERROR);
	return EXIT_FAILURE;
    }
    std::size_t p = cmd.find(cmd[0] == L'"' ? L'"' : L' ', 1);
    if (p == cmd.npos) {
	MessageBox(0, L"Invalid command line", L"kiosh", MB_ICONERROR);
	return EXIT_FAILURE;
    }
    cmd.erase(0, p + 1);

    // FIXME: Get this ugliness out of the main application code
    STARTUPINFO si = {cb: sizeof(si)};
    PROCESS_INFORMATION pi;
    CreateProcess(0, const_cast< wchar_t* >(cmd.c_str()), 0, 0, FALSE, 0, 0, 0, &si, &pi);

    return EXIT_SUCCESS;
}
