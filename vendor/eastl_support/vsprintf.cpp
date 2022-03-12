#include <EABase/eabase.h>

#include <stdio.h>
#include <wchar.h>

// User-provided functions.
int Vsnprintf8 (char* pDestination, size_t n, const char*  pFormat, va_list arguments) {
	return vsnprintf(pDestination, n, pFormat, arguments);
}

int Vsnprintf16(char16_t* pDestination, size_t n, const char16_t* pFormat, va_list arguments) {
	return vswprintf(reinterpret_cast<wchar_t*>(pDestination), n, reinterpret_cast<const wchar_t*>(pFormat), arguments);
}

int Vsnprintf32(char32_t* pDestination, size_t n, const char32_t* pFormat, va_list arguments) {
	// unsupported
	return -1;
}

// Ideally speaking, this will be true on all compiler/versions we support.
#if EA_CHAR8_UNIQUE
int Vsnprintf8 (char8_t* pDestination, size_t n, const char8_t*  pFormat, va_list arguments) {
	return vsnprintf(reinterpret_cast<char*>(pDestination), n, reinterpret_cast<const char*>(pFormat), arguments);
}
#endif

#if defined(EA_WCHAR_UNIQUE) && EA_WCHAR_UNIQUE
int VsnprintfW(wchar_t* pDestination, size_t n, const wchar_t* pFormat, va_list arguments) {
	return vswprintf(pDestination, n, pFormat, arguments);
}
#endif