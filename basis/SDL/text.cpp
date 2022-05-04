#include "text.hpp"          // 放最前面以兼容 maxOS

#include <cstdarg>

using namespace WarGrey::STEM;

/*************************************************************************************************/
#define VSNWPRINT(pool, size, fmt) \
    static wchar_t pool[size]; \
    va_list argl; \
    va_start(argl, fmt); \
    vswprintf(pool, size, fmt, argl); \
    va_end(argl);

#define VSWPRINT(retval, fmt) \
    const int DEFAULT_POOL_SIZE = 1024; \
    wchar_t wpool[DEFAULT_POOL_SIZE]; \
    int bigSize = DEFAULT_POOL_SIZE - 1; \
    wchar_t* pool; \
    va_list argl; \
    do { \
	pool = (bigSize < DEFAULT_POOL_SIZE) ? wpool : (new wchar_t[bigSize + 1]); \
    	va_start(argl, fmt); \
    	int status = vswprintf(pool, bigSize + 1, fmt, argl); \
	va_end(argl); \
        if ((status == -1) || (status > bigSize)) { \
	    bigSize = ((status > 0) ? status : (bigSize * 2)) + 1; \
	    if (pool != wpool) delete[] pool; \
	    pool = nullptr; \
	} \
    } while (pool == nullptr); \
    std::wstring retval(pool); \
    if (pool != wpool) delete[] pool;

#define VSNPRINT(retval, fmt) \
    const int DEFAULT_POOL_SIZE = 1024; \
    char chpool[DEFAULT_POOL_SIZE]; \
    int bigSize = DEFAULT_POOL_SIZE - 1; \
    char* pool; \
    va_list argl; \
    do { \
	pool = (bigSize < DEFAULT_POOL_SIZE) ? chpool : (new char[bigSize + 1]); \
    	va_start(argl, fmt); \
    	int status = vsnprintf(pool, bigSize + 1, fmt, argl); \
	va_end(argl); \
        if ((status == -1) || (status > bigSize)) { \
	    bigSize = ((status > 0) ? status : (bigSize * 2)) + 1; \
	    if (pool != chpool) delete[] pool; \
	    pool = nullptr; \
	} \
    } while (pool == nullptr); \
    std::string retval(pool); \
    if (pool != chpool) delete[] pool;

/*************************************************************************************************/
std::string WarGrey::STEM::game_create_nstring(const char* fmt, ...) {
    VSNPRINT(s, fmt);

    return s;
}

std::wstring WarGrey::STEM::game_create_wstring(const wchar_t* wfmt, ...) {
    VSWPRINT(ws, wfmt);

    return ws;
}

/*************************************************************************************************/
void WarGrey::STEM::game_text_size(TTF_Font* font, int* width, int* height, const char* fmt, ...) {
}

