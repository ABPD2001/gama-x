#ifndef TERMINAL_HPP
#define TERMINAL_HPP 
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <cstdint>
#include <string>

using std::string;
using std::to_string;

// PRIMARY KEYS MARCROS

#define ESC '\033'
#define CSI ESC+'['

// MODES MACROS

#define TEXT_BOLD 1
#define TEXT_DEFAULT 0
#define TEXT_UNDERLINE 4
#define TEXT_STRIKE_THR 9
#define TEXT_FAINT 2
#define TEXT_ITALIC 3
#define TEXT_BLINKING 5
#define TEXT_INVERSE 7
#define TEXT_HIDDEN 8

// COLORS MACROS

#define TEXT_COLOR_BLACK 30
#define TEXT_COLOR_RED 31
#define TEXT_COLOR_GREEN 32
#define TEXT_COLOR_YELLOW 33
#define TEXT_COLOR_BLUE 34
#define TEXT_COLOR_PURPLE 35
#define TEXT_COLOR_CYAN 36
#define TEXT_COLOR_WHITE 37
#define TEXT_COLOR_DEFAULT 39

// BACKGROUND COLORS MACROS

#define TEXT_BACKGROUND_COLOR_BLACK 40
#define TEXT_BACKGROUND_COLOR_RED 41
#define TEXT_BACKGROUND_COLOR_GREEN 42
#define TEXT_BACKGROUND_COLOR_YELLOW 43
#define TEXT_BACKGROUND_COLOR_BLUE 44
#define TEXT_BACKGROUND_COLOR_MAGENTA 45
#define TEXT_BACKGROUND_COLOR_CYAN 46
#define TEXT_BACKGROUND_COLOR_WHITE 47
#define TEXT_BACKGROUND_COLOR_DEFAULT 49

using std::cout;

namespace clig {
struct Color_t {
        const uint8_t color = TEXT_COLOR_DEFAULT;
        const uint8_t background = TEXT_BACKGROUND_COLOR_DEFAULT;
        const uint8_t mode = TEXT_DEFAULT;
};

inline void _setCursor(uint32_t x, uint16_t y);
inline void _resetCursor();
bool setRawMode(bool mode);
inline void _setColor(const uint8_t color, const uint8_t bkgColor = TEXT_BACKGROUND_COLOR_DEFAULT, const uint8_t mode = TEXT_DEFAULT);
inline void _resetColor();
};

#endif
