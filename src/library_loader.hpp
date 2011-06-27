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

class library_loader;

// Can't use std::function as it doesn't support WINAPI (__attribute__((__stdcall__)) on Win32)
namespace {
    template< typename T >
    struct function_traits;

    template< typename Ret, typename... Args >
    struct function_traits< Ret(Args...) > {
	typedef Ret return_type;
    };

#if !defined(__WIN64__)
    template< typename Ret, typename... Args >
    struct function_traits< Ret WINAPI(Args...) > {
	typedef Ret return_type;
    };
#endif

    template< typename F >
    struct function {
	// Can't use decltype foo because of GCC ICE.  FIXME: check if this still a problem with GCC > 4.4.5? and if so, file a bug
	template< typename... Args >
	typename function_traits< F >::return_type operator ()(Args&&... args) const { return this->f(std::forward< Args >(args)...); }

	/*explicit*/ operator bool() const { return this->f; }

    private:
	friend class ::library_loader;
	template< typename G >
	function(G* g) : f{reinterpret_cast< F* >(g)} {}
	F* f;
    };
}

class library_loader
{
public:
    library_loader(const std::wstring& name);
    library_loader(const wchar_t* name);

    /*explicit*/ operator bool() const { return h; }

    template< typename F >
    function< F > symbol(const std::string& name) const {
	return GetProcAddress(h.get(), name.c_str());
    }

    template< typename F >
    function< F > symbol(unsigned int ordinal) const {
	return GetProcAddress(h.get(), reinterpret_cast< const char* >(ordinal));
    }

private:
    std::shared_ptr< std::remove_pointer< HMODULE >::type > h;
};

#endif
