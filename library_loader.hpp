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

#ifndef LIBRARY_LOADER_HPP
#define LIBRARY_LOADER_HPP

#include <memory>
#include <string>
#include <type_traits>

#include <windows.h>

class library_loader
{
public:
    library_loader(const std::wstring& name);
    library_loader(const wchar_t* name);

    operator bool() const { return h; }

    template< typename Ret, typename... Args >
    struct function
    {
	typedef WINAPI Ret (*F)(Args...);

	function(F _f) : f{_f} {}

	operator bool() const { return f; }
	Ret operator ()(Args... args) const { return f(args...); }

    private:
	friend class library_loader;
	template< typename Generic >
	function(Generic _f) : f{reinterpret_cast< F >(_f)} {}

	F f;
    };

    template< typename Ret, typename... Args >
    function< Ret, Args... > symbol(const std::string& name) {
	return GetProcAddress(h.get(), name.c_str());
    }

    template< typename Ret, typename... Args >
    function< Ret, Args... > symbol(unsigned int ordinal) {
	return GetProcAddress(h.get(), reinterpret_cast< const char* >(ordinal));
    }

private:
    std::shared_ptr< std::remove_pointer< HMODULE >::type > h;
};

#endif
