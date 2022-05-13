#include "game.hpp"                 // 放最前面以兼容 macOS
#include "text.hpp"

#include <cstdarg>
#include <iostream>

using namespace WarGrey::STEM;

/*************************************************************************************************/
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
std::string WarGrey::STEM::game_create_string(const char* fmt, ...) {
    VSNPRINT(s, fmt);

    return s;
}

/*************************************************************************************************/
void WarGrey::STEM::game_text_size(TTF_Font* font, int* width, int* height, const char* fmt, ...) {
    VSNPRINT(text, fmt);

    if (font == NULL) {
        font = GAME_DEFAULT_FONT;
    }

    if (TTF_SizeUTF8(font, text.c_str(), width, height)) {
        std::cerr << "无法计算文本尺寸: " << TTF_GetError() << std::endl;
    }
}

