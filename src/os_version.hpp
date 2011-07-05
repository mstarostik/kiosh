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

#ifndef OS_VERSION_HPP
#define OS_VERSION_HPP

#include <windows.h>

class os_version {
public:
    os_version();

    DWORD major() { return d.dwMajorVersion; }
    DWORD minor() { return d.dwMinorVersion; }
    DWORD build() { return d.dwBuildNumber; }

private:
    OSVERSIONINFOEX d;
};

#endif
