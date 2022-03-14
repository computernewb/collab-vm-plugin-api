//
// CollabVM Plugin API
//
// This file is licensed under the GNU Lesser General Public License Version 3
// for external usage in plugins. Text is provided in LICENSE-PLUGIN.
// If you cannot access it, please visit https://www.gnu.org/licenses/lgpl-3.0.html.
//

// This implements the non-EAStdC versions of the Vsnprintf* functions,
// so we can use EASTL inside of plugins, without bringing in EAStdC, and
// the package-a-many that EAStdC depends on.
//
// Both the CollabVM Server and plugins will link to this library
// when linking with the EASTL library.
//
// There's no header for this library, EASTL imports all the functions as it needs.


#include <EABase/eabase.h>

#include <stdio.h>
#include <wchar.h>

int Vsnprintf8(char* pDestination, size_t n, const char* pFormat, va_list arguments) {
	return vsnprintf(pDestination, n, pFormat, arguments);
}

int Vsnprintf16(char16_t* pDestination, size_t n, const char16_t* pFormat, va_list arguments) {
	// This assumes your system C library is aligned to mean
	// wchar_t == UTF-16.
	// On Windows, (the only platform this is relevant for), this assertion seems to be true.
	// Idk about linux.
	return vswprintf(reinterpret_cast<wchar_t*>(pDestination), n, reinterpret_cast<const wchar_t*>(pFormat), arguments);
}

int Vsnprintf32(char32_t* pDestination, size_t n, const char32_t* pFormat, va_list arguments) {
	// unsupported, unless some code path we're hitting ACTUALLY uses this.
	return -1;
}

// Ideally speaking, this will be true on all compiler/versions we support,
// since we're compiling with C++20. But we keep it in EABase wrapping just in case.
#if EA_CHAR8_UNIQUE
int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments) {
	return vsnprintf(reinterpret_cast<char*>(pDestination), n, reinterpret_cast<const char*>(pFormat), arguments);
}
#endif

#if defined(EA_WCHAR_UNIQUE) && EA_WCHAR_UNIQUE
int VsnprintfW(wchar_t* pDestination, size_t n, const wchar_t* pFormat, va_list arguments) {
	return vswprintf(pDestination, n, pFormat, arguments);
}
#endif