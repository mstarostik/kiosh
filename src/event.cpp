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

#include "event.hpp"

event::event(const std::wstring& name)
    : h{OpenEvent(EVENT_MODIFY_STATE, FALSE, name.c_str()), CloseHandle} {
}

event::event(const wchar_t* name)
    : h{OpenEvent(EVENT_MODIFY_STATE, FALSE, name), CloseHandle} {
}

void event::set() {
    SetEvent(h.get());
}

void event::reset() {
    ResetEvent(h.get());
}

void event::pulse() {
    PulseEvent(h.get());
}
